#include "logger.h"

int main(int argc, char **argv)
{
    setup_logger(stdout, LOG_LEVEL_DEBUG);  

    LOGGING_FATAL("test 1 = %d %d", 123, 456);
    LOGGING_FATAL("test 2 = %s %d", "tttttest", 555);
    LOGGING_ERROR("test 3 = %s %s", "hihi", "hoho");
    LOGGING_WARN("test 4","");
    LOGGING_INFO("test 5","");
    LOGGING_DEBUG("test 6","");

    return 0;
}