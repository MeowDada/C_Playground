#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <string.h>

#define LOG_LEVEL_FATAL   (0)
#define LOG_LEVEL_ERROR   (1)
#define LOG_LEVEL_WARN    (2)
#define LOG_LEVEL_INFO    (3)
#define LOG_LEVEL_DEBUG   (4)

extern FILE  *_default_log_stream;
extern int    _default_log_level;

int setup_logger(FILE* fd, int log_level);

void log_msg(int level, const char *file, int line, const char *func, 
    const char *format, ...);

#define LOGGING_FATAL(...)  \
{   \
    log_msg(LOG_LEVEL_FATAL, __FILE__, __LINE__, __func__, __VA_ARGS__);  \
}   \

#define LOGGING_ERROR(...)  \
{   \
    log_msg(LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__); \
}   \

#define LOGGING_WARN(...)   \
{   \
    log_msg(LOG_LEVEL_WARN, __FILE__, __LINE__, __func__, __VA_ARGS__);  \
}   \

#define LOGGING_INFO(...)   \
{   \
    log_msg(LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__);  \
}   \

#define LOGGING_DEBUG(...)  \
{   \
    log_msg(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__);  \
}   \

#endif  /* LOGGER_H */