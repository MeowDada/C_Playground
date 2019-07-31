#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void try_assign_double_pointer(int *single_ptr, int **double_ptr)
{
    LOGGING_INFO("Try assign *double_ptr = single_ptr");
    *double_ptr = single_ptr;
    LOGGING_INFO("Assign *double_ptr = single_ptr not yet crashed");
}

int main(int argc, char **argv)
{
    setup_logger(NULL, LOG_LEVEL_INFO);

    int   value      = 5;
    int  *single_ptr = &value;
    int **double_ptr = malloc(sizeof(int **));
    LOGGING_INFO("Initialize int   value      = %d", value);
    LOGGING_INFO("           int  *single_ptr = &value");
    LOGGING_INFO("           int **double_ptr = NULL");

    try_assign_double_pointer(single_ptr, double_ptr);

    if (*double_ptr) {
        LOGGING_INFO("&value       = %p", &value);
        LOGGING_INFO("*double_ptr  = %p", *double_ptr);
        LOGGING_INFO("**double_ptr = %d", **double_ptr);
    }
    else {
        LOGGING_INFO("Cannot dereference *double_ptr, because it is a NULL pointer");
    }

    free(double_ptr);

    return 0;
}