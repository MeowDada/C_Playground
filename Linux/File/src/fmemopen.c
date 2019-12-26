#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char **argv) {

    char *mem_stream = 
    "Hello, this is a memory stream!\0\0\0\0\0\0\0\0\n"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\n"
    "Also, it is a sparse file with holes.\n"
    "\0\0\0\0\0\0\0\0\0\0\0\0Just check for the follwing\n"
    "test";

    size_t stream_len = 148;

    printf("memory stream size = %lu bytes\n", stream_len);

    FILE *fp = fmemopen(mem_stream, stream_len, "r+");
    if (!fp ) {
        fprintf(stderr, "Failed to fmemopen");
        return EXIT_FAILURE;
    }

    int fd = fileno(fp);

    printf("fd = %d\n", fd);

    off_t seek_off = lseek(fd, 10, SEEK_CUR);
    printf("seek_off = %ld\n", seek_off);

    seek_off = lseek(fd, 10, SEEK_CUR);
    printf("seek_off = %ld\n", seek_off);

    return EXIT_SUCCESS;
}