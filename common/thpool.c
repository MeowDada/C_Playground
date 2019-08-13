#if defined(__linux__)
#include <sys/prctl.h>
#endif
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include "thpool.h"

static volatile int threads_keepalive;
static volatile int threads_on_hold;

typedef struct thpool_job_t thpool_job_t;

typedef struct binary_semaphore_t {
    pthread_mutex_t mutex;
    pthread_cond_t   cond;
    int v;
} binary_semaphore_t;

typedef struct thpool_job_t {
    thpool_job_t  *prev;
    void         (*function)(int id, void *arg);
    void          *args;
} thpool_job_t;

typedef struct thpool_job_queue_t {
    pthread_mutex_t     rw_mutex;
    thpool_job_t       *front;
    thpool_job_t       *rear;
    binary_semaphore_t *bsem_has_job;
    int                 len;
} thpool_job_queue_t;

typedef struct thread_t {
    int           id;
    pthread_t     pthread;
    threadpool_t *thread_pool;
} thread_t;

typedef struct thpool_t {
    thread_t         **threads;
    volatile int       num_threads_alive;
    volatile int       num_threads_working;
    pthread_mutex_t    thcount_lock;
    pthread_cond_t     threads_all_idle;
    thpool_job_queue_t job_queue;
} thpool_t;

static int           thread_init(thpool_t *thpool, thread_t **thread, int id);
static void         *thread_do(thread_t *thread);
static void          thread_hold(int signal);
static void          thread_destroy(thread_t *thread);

static int           jobqueue_init(thpool_job_queue_t *jobq);
static void          jobqueue_clear(thpool_job_queue_t *jobq);
static void          jobqueue_push(thpool_job_queue_t *jobq, thpool_job_t *job);
static thpool_job_t *jobqueue_pull(thpool_job_queue_t *jobq);
static void          jobqueue_destroy(thpool_job_queue_t *jobq);

static void          bsem_init(binary_semaphore_t *bsem, int value);
static void          bsem_reset(binary_semaphore_t *bsem);
static void          bsem_post(binary_semaphore_t *bsem);
static void          bsem_post_all(binary_semaphore_t *bsem);
static void          bsem_wait(binary_semaphore_t *bsem);

threadpool_t *thpool_init(int num_threads)
{
    threads_on_hold   = 0;
    threads_keepalive = 1;

    if (num_threads < 0)
        num_threads = 0;
    
    /* Create a new thread pool */
    threadpool_t *thread_pool = NULL;
    thread_pool = (threadpool_t *)malloc(sizeof(threadpool_t));
    if (!thread_pool) {
        fprintf(stderr, "thpool_init(): Could not allocate memory for thread pool\n");
        return NULL;
    }
    thread_pool->num_threads_alive   = 0;
    thread_pool->num_threads_working = 0;

    /* Initialize the job queue */
    if (jobqueue_init(&thread_pool->job_queue) == -1) {
        fprintf(stderr, "thpool_init(): Could not allocate memory for job queue\n");
        free(thread_pool);
        return NULL;
    }

    /* Create threads for thread pool */
    thread_pool->threads = (thread_t **)calloc(num_threads, sizeof(thread_t *));
    if (!thread_pool->threads) {
        fprintf(stderr, "thpool_init(): Could not allocate memory for threads\n");
        jobqueue_destroy(&thread_pool->job_queue);
        free(thread_pool);
        return NULL;
    }

    pthread_mutex_init(&thread_pool->thcount_lock, NULL);
    pthread_cond_init(&thread_pool->threads_all_idle, NULL);

    /* initialize all threads in this thread pool */
    int n = 0;
    for (n = 0; n < num_threads; n++)
        thread_init(thread_pool, &thread_pool->threads[n], n);

    while (thread_pool->num_threads_alive != num_threads) {}

    return thread_pool;
}

int thpool_add_work(threadpool_t *thread_pool, void (*funcptr)(int, void *), void *args)
{
    thpool_job_t *job = (thpool_job_t *)malloc(sizeof(thpool_job_t));
    if (!job) {
        fprintf(stderr, "thpool_add_work(): Could not allocate memory for new job\n");
        return -1;
    }

    job->function = funcptr;
    job->args     = args;

    jobqueue_push(&thread_pool->job_queue, job);

    return 0;
}

void thpool_wait(threadpool_t *thread_pool)
{
    if (!thread_pool)
        return;
    
    pthread_mutex_lock(&thread_pool->thcount_lock);
    while (thread_pool->job_queue.len || thread_pool->num_threads_working)
        pthread_cond_wait(&thread_pool->threads_all_idle, &thread_pool->thcount_lock);
    pthread_mutex_unlock(&thread_pool->thcount_lock);
}

void thpool_destroy(threadpool_t *thread_pool)
{
    if (!thread_pool)
        return;
    
    volatile int threads_total = thread_pool->num_threads_alive;

    threads_keepalive = 0;

    double TIMEOUT = 1.0;
    time_t start, end;
    double elapsed = 0.0;

    /* Give TIMEOUT seconds to kill idle thread */
    time(&start);
    while (elapsed < TIMEOUT && thread_pool->num_threads_alive) {
        bsem_post_all(thread_pool->job_queue.bsem_has_job);
        time(&end);
        elapsed = difftime(end, start);
    }

    /* Pool remainging threads */
    while (thread_pool->num_threads_alive) {
        bsem_post_all(thread_pool->job_queue.bsem_has_job);
        sleep(1);
    }

    jobqueue_destroy(&thread_pool->job_queue);

    int n = 0;
    for (n = 0; n < threads_total; n++)
        thread_destroy(thread_pool->threads[n]);
    
    free(thread_pool->threads);
    free(thread_pool);
}

void thpool_pause(threadpool_t *thread_pool)
{
    int n = 0;
    for (n = 0; n < thread_pool->num_threads_alive; n++) {
        pthread_kill(thread_pool->threads[n]->pthread, SIGUSR1);
    }
}

void thpool_resume(threadpool_t *thread_pool)
{
    (void)thread_pool;

    threads_on_hold = 0;
}

int thpool_num_threads_working(threadpool_t *thread_pool)
{
    return thread_pool->num_threads_working;
}

static int thread_init(threadpool_t *thread_pool, thread_t **thread, int id)
{
    pthread_attr_t attr;
    int ret = 0;
    int stack_size = 1048576;

    *thread = (thread_t *)malloc(sizeof(thread_t));
    if (!(*thread)) {
        fprintf(stderr, "thread_init(): Could not allocate memoery for thread\n");
        return -1;
    }

    ret = pthread_attr_init(&attr);
    if (ret != 0) {
        fprintf(stderr, "pthread_attr_init(): Could not initialize attr\n");
        free(*thread);
        return -1;
    }

    ret = pthread_attr_setstacksize(&attr, stack_size);
    if (ret != 0) {
        fprintf(stderr, "pthread_attr_setstacksize(): Could not set stack size\n");
        free(*thread);
        return -1;
    }

    (*thread)->thread_pool = thread_pool;
    (*thread)->id          = id;

    pthread_create(&(*thread)->pthread, &attr, (void *)thread_do, (*thread));
    pthread_detach((*thread)->pthread);

    ret = pthread_attr_destroy(&attr);

    return 0;
}

static void thread_hold(int signal)
{
    (void)signal;

    threads_on_hold = 1;
    while (threads_on_hold)
        sleep(1);
}

static void *thread_do(thread_t *thread)
{
    char thread_name[128] = {0};
    sprintf(thread_name, "thread-pool-%d", thread->id);

#if defined(__linux__)
    prctl(PR_SET_NAME, thread_name);
#else
    fprintf(stderr, "thread_do(): pthread_setname is not supported on this system");
#endif

    threadpool_t *thread_pool = thread->thread_pool;
#if defined(__linux__)
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = thread_hold;
    if (sigaction(SIGUSR1, &act, NULL) == -1) {
        fprintf(stderr, "thread_do(): cannot handle SIGUSR1");
    }
#endif

    pthread_mutex_lock(&thread_pool->thcount_lock);
    thread_pool->num_threads_alive += 1;
    pthread_mutex_unlock(&thread_pool->thcount_lock);

    while (threads_keepalive) {
        bsem_wait(thread_pool->job_queue.bsem_has_job);

        if (threads_keepalive) {
            pthread_mutex_lock(&thread_pool->thcount_lock);
            thread_pool->num_threads_working++;
            pthread_mutex_unlock(&thread_pool->thcount_lock);

            void (*func_buff)(int, void *) = NULL;
            void *arg_buff = NULL;

            /* Read job from queue and execute it */
            thpool_job_t *job = jobqueue_pull(&thread_pool->job_queue);
            if (job) {
                func_buff = job->function;
                arg_buff  = job->args;
                func_buff(thread->id, arg_buff);
                free(job);
            }

            pthread_mutex_lock(&thread_pool->thcount_lock);
            thread_pool->num_threads_working--;
            if (!thread_pool->num_threads_working) {
                pthread_cond_signal(&thread_pool->threads_all_idle);
            }
            pthread_mutex_unlock(&thread_pool->thcount_lock);
        }
    }
    pthread_mutex_lock(&thread_pool->thcount_lock);
    thread_pool->num_threads_alive--;
    pthread_mutex_unlock(&thread_pool->thcount_lock);

    return NULL;
}

static void thread_destroy(thread_t *thread)
{
    free(thread);
}

static int jobqueue_init(thpool_job_queue_t *jobq)
{
    jobq->len   = 0;
    jobq->front = NULL;
    jobq->rear  = NULL;

    jobq->bsem_has_job = (binary_semaphore_t *)malloc(sizeof(binary_semaphore_t));
    if (!jobq->bsem_has_job) {
        return -1;
    }

    pthread_mutex_init(&jobq->rw_mutex, NULL);
    bsem_init(jobq->bsem_has_job, 0);

    return 0;
}

static void jobqueue_clear(thpool_job_queue_t *jobq)
{
    while (jobq->len) {
        free(jobqueue_pull(jobq));
    }

    jobq->front = NULL;
    jobq->rear  = NULL;
    bsem_reset(jobq->bsem_has_job);
    jobq->len   = 0;
}

static void jobqueue_push(thpool_job_queue_t *jobq, thpool_job_t *job)
{
    pthread_mutex_lock(&jobq->rw_mutex);
    job->prev = NULL;

    switch (jobq->len)
    {
        case 0:
            jobq->front = job;
            jobq->rear  = job;
            break;
        default:
            jobq->rear->prev = job;
            jobq->rear = job;
            break;
    }

    jobq->len++;

    bsem_post(jobq->bsem_has_job);
    pthread_mutex_unlock(&jobq->rw_mutex);
}

static thpool_job_t *jobqueue_pull(thpool_job_queue_t *jobq)
{
    pthread_mutex_lock(&jobq->rw_mutex);
    thpool_job_t *job = jobq->front;

    switch (jobq->len)
    {
        case 0:
            break;
        case 1:
            jobq->front = NULL;
            jobq->rear  = NULL;
            jobq->len   = 0;
            break;
        default:
            jobq->front = job->prev;
            jobq->len--;
            bsem_post(jobq->bsem_has_job);
    }
    
    pthread_mutex_unlock(&jobq->rw_mutex);
    return job;
}

static void jobqueue_destroy(thpool_job_queue_t *jobq)
{
    jobqueue_clear(jobq);
    free(jobq->bsem_has_job);
}

static void bsem_init(binary_semaphore_t *bsem, int value)
{
    if (value < 0 || value > 1) {
        fprintf(stderr, "bsem_init(): Binary semaphore can take only value 1 or 0");
        exit(1);
    }
    pthread_mutex_init(&bsem->mutex, NULL);
    pthread_cond_init(&bsem->cond, NULL);
    bsem->v = value;
}

static void bsem_reset(binary_semaphore_t *bsem)
{
    bsem_init(bsem, 0);
}

static void bsem_post(binary_semaphore_t *bsem)
{
    pthread_mutex_lock(&bsem->mutex);
    bsem->v = 1;
    pthread_cond_signal(&bsem->cond);
    pthread_mutex_unlock(&bsem->mutex);
}

static void bsem_post_all(binary_semaphore_t *bsem)
{
    pthread_mutex_lock(&bsem->mutex);
    bsem->v = 1;
    pthread_cond_broadcast(&bsem->cond);
    pthread_mutex_unlock(&bsem->mutex);
}

static void bsem_wait(binary_semaphore_t *bsem)
{
    pthread_mutex_lock(&bsem->mutex);
    while (bsem->v != 1) {
        pthread_cond_wait(&bsem->cond, &bsem->mutex);
    }
    bsem->v = 0;
    pthread_mutex_unlock(&bsem->mutex);
}
