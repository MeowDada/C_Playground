#include <stdio.h>
#include <stdlib.h>

#include "thpool.h"
#include "logger.h"

typedef struct handle_t {
    int number;
    int limit;
} handle_t;

void increment(int id, void *args)
{
    handle_t *handle = (handle_t *)args;

    while(1) {
        if (handle->number < handle->limit) {
            handle->number++;
            LOGGING_DEBUG("thpool-%d: handle->number = %4d",id ,handle->number);
        }
        else {
            break;
        }
    }
}

int main(int argc, char **argv)
{
    setup_logger(NULL, LOG_LEVEL_DEBUG);

    threadpool_t *thpool = thpool_init(8);

    int number = 0;
    int limit  = 100;

    handle_t handle; 
    handle.number = number;
    handle.limit  = limit;

    thpool_add_work(thpool, increment, &handle);
    thpool_add_work(thpool, increment, &handle);
    thpool_add_work(thpool, increment, &handle);

    close_logger(NULL);

    return 0;
}