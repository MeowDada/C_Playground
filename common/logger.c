#include "logger.h"

FILE  *_default_log_stream = NULL;
int    _default_log_level  = LOG_LEVEL_ERROR;

int setup_logger(FILE* fd, int log_level)
{
    if (!fd) {
        fprintf(stderr, "Invalid file descriptor");
        return -1;
    }

    _default_log_stream = fd;
    _default_log_level  = log_level;

    return 0;
}