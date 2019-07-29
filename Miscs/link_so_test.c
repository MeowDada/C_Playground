#include "common.h"

int main(int argc, char **argv)
{
    setup_logger(NULL, LOG_LEVEL_DEBUG);

    LOGGING_DEBUG("Test for linking .so");
    LOGGING_INFO("JUST A TEST");

    return 0;
}