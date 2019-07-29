#include <stdio.h>
#include <stdlib.h>

/* by doing this is able to avoid using dangling pointer */
void free_pointer_correctly(void **ptr)
{
    void *tmp_ptr = *ptr;
    free(tmp_ptr);
    *ptr = NULL;
}

int main(int argc, char **argv)
{
    int value = 5;
    int *ptr = &value;
    free_pointer_correctly((void **)&ptr);

    if (ptr) {
        fprintf(stdout, "ptr is not NULL after freeing!\n");
    }
    else {
        fprintf(stdout, "ptr is NULL after freeing!\n");
    }

    return 0;
}