#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include "common.h"

#define DEFAULT_GENERATE_NUMBER_RANGE        (100)

typedef struct buffer_t {
    unsigned int   num_gen;
    unsigned int   num_con;
    size_t         capacity;
    size_t         size;
    int           *data;
    int           *dataptr;
    sem_t          mutex;
    sem_t          empty;
    sem_t          full;
} buffer_t;

typedef struct thread_info_t {
    pthread_t thread;
    pthread_attr_t *attr;
    void *(*routine)(void *);
} thread_info_t;

typedef struct handle_t {
    buffer_t *buffer;
    thread_info_t *thread_info;
} handle_t;

int max_generate = 100;

static const char *red    = "\033[1;31m";
static const char *green  = "\033[0;32m";
static const char *yellow = "\033[1;33m";
static const char *reset  = "\033[0m"; 

static buffer_t *buffer_create(int capacity)
{
    buffer_t *buffer = (buffer_t *)malloc(sizeof(buffer_t));
    if (!buffer) {
        LOGGING_ERROR("failed to create buffer");
        return NULL;
    }

    buffer->data = (int *)malloc(capacity*sizeof(int));
    if (!buffer->data) {
        LOGGING_ERROR("failed to malloc buffer");
        free(buffer);
        return NULL;
    }

    buffer->capacity = (size_t)capacity;
    buffer->size     = 0;
    buffer->dataptr  = buffer->data;
    buffer->num_gen  = 0;
    buffer->num_con  = 0;

    sem_init(&buffer->mutex, 0, 1);
    sem_init(&buffer->full, 0, 0);
    sem_init(&buffer->empty,  0, capacity);

    return buffer;
}

static void buffer_destroy(buffer_t *buffer)
{
    if (!buffer) {
        LOGGING_WARN("do nothing because input is NULL buffer pointer");
        return;
    }

    free(buffer->data);
    buffer->data    = NULL;
    buffer->dataptr = NULL;

    sem_destroy(&buffer->mutex);
    sem_destroy(&buffer->empty);
    sem_destroy(&buffer->full);
}

static thread_info_t *threads_create(int num_pthread, pthread_attr_t *attr,
    void *(*routine)(void *), void *args)
{
    thread_info_t *threads = (thread_info_t *)calloc(num_pthread, sizeof(thread_info_t));
    if (!threads) {
        LOGGING_ERROR("failed to create threads");
        return NULL;
    }

    int i = 0;
    int retval = 0;
    for (i = 0; i < num_pthread; i++) {
        threads[i].attr = attr;
        threads[i].routine = routine;
        handle_t *handle = (handle_t *)malloc(sizeof(handle_t));
        if (!handle)
            return NULL;
        handle->thread_info = &threads[i];
        handle->buffer = (buffer_t *)args;

        retval = pthread_create(&threads[i].thread, attr, routine, (void *)handle);
        if (retval) {
            return NULL;
        }
        retval = pthread_detach(threads[i].thread);
        if (retval) {
            return NULL;
        }
    }

    return threads;
}

static int produce_item(buffer_t *buffer)
{
    int number = rand() % DEFAULT_GENERATE_NUMBER_RANGE;
    memcpy(buffer->dataptr, &number, sizeof(int));
    buffer->size += 1;
    buffer->dataptr += 1;
    buffer->num_gen += 1;
    return number;
}

static int consume_item(buffer_t *buffer)
{
    buffer->size -= 1;
    buffer->dataptr -= 1;
    buffer->num_con += 1;
    int number = *buffer->dataptr;
    return number;
}

static void *do_produce(void *args)
{
    handle_t *handle = (handle_t *)args;
    buffer_t *buffer = handle->buffer;
    thread_info_t *thread_info = handle->thread_info;
    unsigned long tid = pthread_self();

    while (1)
    {
        sem_wait(&buffer->empty);
        sem_wait(&buffer->mutex);

        if (max_generate == buffer->num_gen) {
            LOGGING_INFO("%s[Producer]%s thread#%lu : reach the limit of generateing times, exiting...", red, reset, tid);
            sem_post(&buffer->mutex);
            sem_post(&buffer->full);
            break;
        }

        if (buffer->size < buffer->capacity) {
            int number = produce_item(buffer);
            LOGGING_INFO("%s[Producer]%s thread#%lu : produce %3d, buffer->num_gen = %4d", red, reset, tid, number, buffer->num_gen);
        }
        else {
            LOGGING_INFO("%s[Producer]%s thread#%lu : buffer is full, wait consumer to consume...", red, reset, tid);
        }

        sem_post(&buffer->mutex);
        sem_post(&buffer->full);
    }

    pthread_exit(&thread_info->thread);
    free(handle);
}

static void *do_consume(void *args)
{
    handle_t *handle = (handle_t *)args;
    buffer_t *buffer = handle->buffer;
    thread_info_t *thread_info = handle->thread_info;
    unsigned long tid = pthread_self();

    while (1)
    {
        sem_wait(&buffer->full);
        sem_wait(&buffer->mutex);

        if (buffer->num_con == max_generate) {
            LOGGING_INFO("%s[Consumer]%s thread#%lu : Already consume all generated number, let's exit consumer...", yellow, reset, tid);
            sem_post(&buffer->mutex);
            sem_post(&buffer->empty);
            break;
        }

        int num_con = 0;

        if (buffer->size > 0) {
            int number = consume_item(buffer);
            LOGGING_INFO("%s[Consumer]%s thread#%lu : consume %3d, buffer->num_con = %4d", yellow, reset, tid, number, buffer->num_con);
        }
        else {
            LOGGING_INFO("%s[Consumer]%s thread#%lu : buffer is empty, waiting producer to produce items...", yellow, reset, tid);
        }

        sem_post(&buffer->mutex);
        sem_post(&buffer->empty);
    }
    pthread_exit(&thread_info->thread);
    free(handle);
}

int main(int argc, char **argv)
{
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <log_fname> <num_producer> <num_consumer> <buffer_capacity> <num_generate>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char  *log_fname    = argv[1];
    const int    num_producer = atoi(argv[2]);
    const int    num_consumer = atoi(argv[3]);
    const size_t capacity     = atoi(argv[4]);
    max_generate = atoi(argv[5]);

    FILE *fp = fopen(log_fname, "w+");
    if (!fp)
        return EXIT_FAILURE;

    setup_logger(fp, LOG_LEVEL_DEBUG);

    srand(time(NULL));

    if (num_consumer <= 0 || num_producer <= 0 || max_generate < 0) {
        LOGGING_ERROR("Invalid input");
        close_logger(fp);
        return EXIT_FAILURE;
    }

    buffer_t *buffer = buffer_create(capacity);
    if (!buffer) {
        LOGGING_ERROR("failed to create buffer");
        close_logger(fp);
        return EXIT_FAILURE;
    }
    
    thread_info_t *producers = threads_create(num_producer, NULL, do_produce, (void *)buffer);
    if (!producers) {
        LOGGING_ERROR("failed to create producers");
        goto fail;
    }

    thread_info_t *consumers = threads_create(num_consumer, NULL, do_consume, (void *)buffer);
    if (!consumers) {
        LOGGING_ERROR("failed to create consumers");
        goto fail;
    }

    while (1)
    {
        sem_wait(&buffer->mutex);
        if (buffer->num_con == max_generate && buffer->num_gen == max_generate) {
            LOGGING_INFO("All of the producers and consumers have finished thier jobs, let's exiting...");
            sem_post(&buffer->mutex);
            break;
        }
        sem_post(&buffer->mutex);
    }

    free(producers);
    free(consumers);

end:
    buffer_destroy(buffer);
    buffer = NULL;

    close_logger(fp);

    return 0;

fail:
    buffer_destroy(buffer);
    buffer = NULL;

    close_logger(fp);

    return EXIT_FAILURE;
}