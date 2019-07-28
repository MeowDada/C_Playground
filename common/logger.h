#include <stdio.h>
#include <string.h>

#define LOG_LEVEL_FATAL   (1)
#define LOG_LEVEL_ERROR   (2)
#define LOG_LEVEL_WARN    (3)
#define LOG_LEVEL_INFO    (4)
#define LOG_LEVEL_DEBUG   (5)

extern FILE  *_default_log_stream;
extern int    _default_log_level;

#define LOGGING(level, format, ...) \
{   \
    if (level >= _default_log_level) {  \
        fprintf(_default_log_stream, format"%10s[%s]: %s #%d\n", __VA_ARGS__, "", __FILE__, __func__, __LINE__);   \
    }   \
}   \

#define LOGGING_FATAL(format, ...)  \
{   \
    LOGGING(_default_log_level, format, __VA_ARGS__);  \
}   \

int setup_logger(FILE* fd, int log_level);

