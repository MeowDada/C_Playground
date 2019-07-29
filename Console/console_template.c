#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "common.h"

static struct option long_options[] = {
    {"log-file",     required_argument,  0, 'L'},
    {"log-level",    optional_argument,  0, 'l'},
    {"display",      no_argument,        0, 'd'},
    {"help",         no_argument,        0, 'h'}
};
static const char *short_options = "L:l::dh";

char        *log_file  = NULL;
static int   log_level = LOG_LEVEL_ERROR; 

void print_usage(const char *program_name)
{
    fprintf(stdout,
"Usage %s [OPTION]...\n"
"This is a console template\n"
"\n"
"[OPTION]\n"
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

int main(int argc, char **argv)
{
    const char *program_name = argv[0];

    parse_commands(program_name, argc, argv);
    setup_configurations();

    return 0;
}