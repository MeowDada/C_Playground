#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void try_assign_double_pointer(int *value, int **double_ptr)
{
    LOGGING_INFO("Try assign *double_ptr = &value");
    if (*double_ptr) {
        *double_ptr = value;
    }
    else {
        LOGGING_INFO("*double_ptr is NULL, cannot assign value to it");
    }
}

int main(int argc, char **argv)
{
    setup_logger(NULL, LOG_LEVEL_INFO);

    int value = 5;
    int **double_ptr = NULL;
    LOGGING_INFO("Initialize int value = %d, int **double_ptr = NULL", value);

    try_assign_double_pointer(&value, double_ptr);

    if (*double_ptr) {
        LOGGING_INFO("*double_ptr = %d", *double_ptr);
    }
    else {
        LOGGING_INFO("Cannot dereference *double_ptr, because it is a NULL pointer");
    }

    return 0;
}