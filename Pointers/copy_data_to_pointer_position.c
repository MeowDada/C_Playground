#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct buf_t {
    char *data;
    char *ptr;
    int capacity;
    int size;
} buf_t;

int main(int argc, char **argv)
{
    printf("Creating a buffer with capacity = sizeof(int) * 10...\n");

    buf_t *buf = (buf_t*)malloc(sizeof(buf_t));
    if (!buf) {
        fprintf(stderr, "Failed to create a buffer instance\n");
        return EXIT_FAILURE;
    }
    
    buf->capacity = sizeof(int) * 10;
    buf->size = 0;
    buf->data = malloc(buf->capacity);
    if (!buf->data) {
        fprintf(stderr, "Failed to create data buffer for the buffer\n");
        return EXIT_FAILURE;
    }
    buf->ptr = buf->data;

    printf("Creating a buffer successfully\n");
    printf("Start copying integer to buffer...\n");

    int i = 0;
    for (i = 0; i < 10; i++) {
        memcpy(buf->ptr, &i, sizeof(int));
        printf("Copying %d to buffer...\n", i);
        printf("The result of recent copied to buffer is %d\n", *(int*)buf->ptr);
        buf->ptr += sizeof(int);
    }

#ifdef _WIN32
    system("pause");
#else
#endif

    return 0;
}