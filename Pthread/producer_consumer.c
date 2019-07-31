#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <time.h>
#include "common.h"

static struct option long_options[] = {
    {"size",         required_argument,  0, 's'},
    {"log-file",     required_argument,  0, 'L'},
    {"log-level",    optional_argument,  0, 'l'},
    {"display",      no_argument,        0, 'd'},
    {"help",         no_argument,        0, 'h'}
};
static const char *short_options = "L:l::dh";

char        *log_file  = NULL;
static int   log_level = LOG_LEVEL_ERROR; 

size_t       buffer_capacity  = 30;

int          num_gen          = 500;
int          num_gen_per_loop = 5;

int          num_has_gen      = 0;
int          num_has_read     = 0;

pthread_mutex_t lock;

void print_usage(const char *program_name)
{
    fprintf(stdout,
"Usage %s [OPTION]...\n"
"This is a console template\n"
"\n"
"[OPTION]\n"
"   -s, --size <value>              Specify buffer size by given <value>\n"
"   -L, --log-file <filename>       Redirect logging stream to the specified file\n"
"   -l, --log-level [<level>]       Set the level of logging to <level>. If not specify <level>,\n"
"                                   the level of logging will be set to LOG_LEVEL_INFO.\n"
"   -d, --display                   Display the configurations\n"
"   -h, --help                      Show this help text\n",
    program_name);
}

void parse_commands(const char *program_name, int argc, char **argv)
{
    int opt = 0;
    while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (opt) {
            case 's':
                buffer_capacity = atoi(optarg);
            case 'L':
                log_file = strdup(optarg);
                break;
            case 'l':
                if (optarg) {
                    log_level = atoi(optarg);
                }
                else {
                    log_level = LOG_LEVEL_INFO;
                }
                break;
            case 'h':
            case '?':
            default:
                print_usage(program_name);
                exit(EXIT_FAILURE);
        }
    }
}

void setup_configurations(void)
{
    FILE *log_fd = NULL;
    if (log_file)
        log_fd = fopen(log_file, "w+");

    if (setup_logger(log_fd, log_level) != 0) {
        fprintf(stderr, "Failed to setup logger!\n");
        exit(EXIT_FAILURE);
    }
}

typedef struct buffer {
    size_t capacity;
    size_t size;
    int *data;
} buf_t;

void *producer_stuff(void *_buf)
{
    buf_t *buf = (buf_t *)_buf;
    int total_gen_num = num_gen;

    while (total_gen_num > 0) {
        pthread_mutex_lock(&lock);
        if (buf->capacity - buf->size > 0) {
            int num_to_gen = rand() % num_gen_per_loop;
            num_to_gen = (num_to_gen > (num_gen - total_gen_num))? (num_gen - total_gen_num) : total_gen_num;
            LOGGING_INFO("[Producer] It's my turn, gonna generate %d numbers...", num_to_gen);
            int i = 0;
            for (i = 0; i < num_to_gen; i++) {
                int idx = buf->size;
                int content = rand() % 10;
                buf->data[idx] = content % 10;   /* assign only 0-9 */
                buf->size++;
            }
            total_gen_num -= num_to_gen;
            num_has_gen += num_to_gen;
        }
        pthread_mutex_unlock(&lock);
    }

    pthread_exit(NULL);
}

void *consumer_stuff(void *_buf)
{
    buf_t *buf = (buf_t *)_buf;

    while (num_has_read != num_gen) {
        pthread_mutex_lock(&lock);
        if (buf->size > 0) {
            LOGGING_INFO("[Consumer] Current read num = %d, [num_has_gen,num_has_read] = [%03d,%03d]", buf->data[buf->size-1], num_has_gen, num_has_read);
            buf->size -= 1;
            num_has_read += 1;
        }
        pthread_mutex_unlock(&lock);
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    const char *program_name = argv[0];

    parse_commands(program_name, argc, argv);
    setup_configurations();

    srand(time(NULL));

    buf_t *buf = malloc(sizeof(buf_t));
    buf->data = malloc(sizeof(int) * buffer_capacity);
    buf->capacity = buffer_capacity;
    buf->size = 0;

    pthread_t producer_thread, consumer_thread;
    pthread_attr_t attr;
    void *status = NULL;
    int retval = 0;

    if (pthread_mutex_init(&lock, NULL) != 0) {
        LOGGING_ERROR("Failed to create mutex");
        return EXIT_FAILURE;
    }


    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* Create producer thread */
    retval = pthread_create(&producer_thread, &attr, producer_stuff, (void *)buf);
    if (retval) {
        LOGGING_ERROR("Failed to create producer thread");
        return EXIT_FAILURE;
    }

    /* Create consumer thread */
    retval = pthread_create(&consumer_thread, &attr, consumer_stuff, (void *)buf);
    if (retval) {
        LOGGING_ERROR("Failed to create consumer thread");
        return EXIT_FAILURE;
    }
    /* pthread_join will block parent thread util the thread is finished */
    pthread_join(consumer_thread, &status);
    pthread_join(producer_thread, &status); 
    pthread_mutex_destroy(&lock); 
    pthread_attr_destroy(&attr);


    return 0;
}