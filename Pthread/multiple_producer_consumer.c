#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include "common.h"
#include "logger.h"

#define DEFAULT_GENERATE_NUMBER_RANGE        (100)
#define PRODUCER_MAX_GENERATE_ITEM_PER_ROUND (7)
#define CONSUMER_MAX_CONSUME_ITEM_PER_ROUND  (5)

typedef struct buffer_t {
    unsigned int   num_gen;
    unsigned int   num_con;
    size_t         capacity;
    size_t         size;
    unsigned char *data;
    unsigned char *dataptr;
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

static buffer_t *buffer_create(size_t capacity, int available)
{
    buffer_t *buffer = (buffer_t *)malloc(sizeof(buffer_t));
    if (!buffer) {
        LOGGING_ERROR("failed to create buffer");
        return NULL;
    }

    buffer->data = (unsigned char *)malloc(capacity*sizeof(int));
    if (!buffer->data) {
        LOGGING_ERROR("failed to malloc buffer");
        free(buffer);
        return NULL;
    }

    buffer->capacity = capacity;
    buffer->size     = 0;
    buffer->dataptr  = buffer->data;
    buffer->num_gen  = 0;
    buffer->num_con  = 0;

    sem_init(&buffer->mutex, 0, 1);
    sem_init(&buffer->empty, 0, 0);
    sem_init(&buffer->full,  0, available);
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
    }

    return threads;
}

static void *do_produce(void *args)
{
    handle_t *handle = (handle_t *)args;
    buffer_t *buffer = handle->buffer;
    thread_info_t *thread_info = handle->thread_info;

    while (1)
    {
        sem_wait(&buffer->empty);
        sem_wait(&buffer->mutex);

        if (max_generate == buffer->num_gen) {
            LOGGING_INFO("[Producer] thread#%d : reach the limit of generateing times, exiting...");
            sem_post(&buffer->mutex);
            sem_post(&buffer->full);
            break;
        }

        int num_gen = 0;

        if (buffer->size < buffer->capacity) {
            num_gen = rand() % PRODUCER_MAX_GENERATE_ITEM_PER_ROUND;
            num_gen = min(num_gen, buffer->capacity - buffer->size);
            num_gen = min(num_gen, max_generate - buffer->num_gen);
            int i = 0;
            int number = 0;
            for (i = 0; i < num_gen; i++) {
                number = rand() % DEFAULT_GENERATE_NUMBER_RANGE;
                memcpy(buffer->dataptr, &number, sizeof(int));
                buffer->dataptr += sizeof(int);
            }
            buffer->size += num_gen;
            buffer->num_gen += num_gen;
            LOGGING_INFO("[Producer] thread#%d : produce %d items, buffer->num_gen = %d", (int)thread_info->thread, num_gen, buffer->num_gen);
        }
        else {
            LOGGING_INFO("[Producer] thread#%d : buffer is full, wait consumer to consume...", (int)thread_info->thread);
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

    while (1)
    {
        sem_wait(&buffer->full);
        sem_wait(&buffer->mutex);

        if (buffer->num_con == max_generate) {
            LOGGING_INFO("[Consumer] thread#%d : Already consume all generated number, let's exit consumer...", (int)thread_info->thread);
            sem_post(&buffer->mutex);
            sem_post(&buffer->empty);
            break;
        }

        int num_con = 0;

        if (buffer->size > 0) {
            num_con = rand() % CONSUMER_MAX_CONSUME_ITEM_PER_ROUND;
            num_con = min(num_con, buffer->size);
            int i = 0;
            for (i = 0; i < num_con ; i++) {
                buffer->dataptr -= sizeof(int);
            }
            buffer->size -= num_con;
            buffer->num_con += num_con;
            LOGGING_INFO("[Consumer] thread#%d : consume %d items, buffer->num_con = %d", (int)thread_info->thread, num_con, buffer->num_con);
        }
        else {
            LOGGING_INFO("[Consumer] thread#%d : buffer is empty, waiting producer to producer items...", (int)thread_info->thread);
        }

        sem_post(&buffer->mutex);
        sem_post(&buffer->empty);
    }
    pthread_exit(&thread_info->thread);
    free(handle);
}

int main(int argc, char **argv)
{
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <num_producer> <num_consumer> <buffer_capacity> <num_generate>", argv[0]);
        return EXIT_FAILURE;
    }

    const int    num_producer = atoi(argv[1]);
    const int    num_consumer = atoi(argv[2]);
    const size_t capacity     = atoi(argv[3]);
    max_generate = atoi(argv[4]);

    setup_logger(stderr, LOG_LEVEL_INFO);

    srand(time(NULL));

    if (num_consumer <= 0 || num_producer <= 0 || max_generate < 0) {
        LOGGING_ERROR("Invalid input");
        close_logger(NULL);
        return EXIT_FAILURE;
    }

    buffer_t *buffer = buffer_create(capacity, num_producer+num_consumer);
    if (!buffer)
        return EXIT_FAILURE;
    
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
        if (buffer->num_con == max_generate && buffer->num_gen) {
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

    close_logger(NULL);

    return 0;

fail:
    buffer_destroy(buffer);
    buffer = NULL;

    close_logger(NULL);

    return EXIT_FAILURE;
}