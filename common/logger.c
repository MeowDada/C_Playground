#include "logger.h"
#include "stdarg.h"

#define MSG_LEN 8192L

FILE  *_default_log_stream   = NULL;
int    _default_log_level    = LOG_LEVEL_ERROR;

const char *log_level_string[] = {
    [LOG_LEVEL_FATAL] = "FATAL",
    [LOG_LEVEL_ERROR] = "ERROR",
    [LOG_LEVEL_WARN]  = "WARN",
    [LOG_LEVEL_INFO]  = "INFO",
    [LOG_LEVEL_DEBUG] = "DEBUG"
};

int setup_logger(FILE *fd, int log_level)
{
    if (!fd)
        _default_log_stream = stderr;
    else
        _default_log_stream = fd;
    
    _default_log_level  = log_level;

    return 0;
}

void close_logger(FILE *fd)
{
    if (fd)
        fclose(fd);
}

#if defined(__MINGW32__)
static void logging_to_file(FILE *fp, int level, char *msg)
{
    fprintf(fp, "%s\n", msg);
}
#else
static void logging_to_file(FILE *fp, int level, char *msg)
{
    fprintf(fp, "%s\n", msg);
}
#endif

static void logging_to_stderr(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

static void do_log_msg(void *zc, int level,
	const char *file, int line, const char *func,
	const char *format, va_list args)
{
    const char *tag = log_level_string[level];
    char msg[MSG_LEN] = { 0 };
    int written = 0;

    /* Level */
    written = snprintf(msg, MSG_LEN, "[%-5s] ", tag);
    if (written < 0)
        return;
    
    written += vsnprintf(msg + written, MSG_LEN - written, format, args);
    if (written < 0)
        return;
    
    if (written >= MSG_LEN - 1)
        goto end;
    
    if (file && line && func) {
        written += snprintf(msg + written, MSG_LEN - written,
            "   (%s:%d:%s)", file, line, func);
        if (written < 0)
            return;
    }

    msg[written] = '\0';
end:
    if (zc) {
        logging_to_file((FILE *)zc, level, msg);
    }
    else {
        logging_to_stderr(msg);
    }
}

void log_msg(void *zc, int level, const char *file, int line,
    const char *func, const char *format, ...)
{
    va_list argptr;

    if (level > _default_log_level)
        return;
    
    va_start(argptr, format);
    do_log_msg(zc, level, file, line, func, format, argptr);
    va_end(argptr);
}