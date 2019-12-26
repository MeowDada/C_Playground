#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    const char *prog_name = argv[0];

    if (argc < 2) {
        fprintf(stderr, "Please input a file name: %s <filename>\n", prog_name);
        return EXIT_FAILURE;
    }

    const char *file = argv[1];
    if (!file) {
        fprintf(stderr, "file is a null pointer.\n");
        return EXIT_FAILURE;
    }

    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open(%s): %s\n", file, strerror(errno));
        return EXIT_FAILURE;
    }

    off_t seek_off = lseek(fd, 10, SEEK_CUR);
    printf("lseek(fd, 10, SEEK_CUR) = %ld\n", seek_off);

    seek_off = lseek(fd, 10, SEEK_CUR);
    printf("lseek(fd, 10, SEEK_CUR) = %ld\n", seek_off);

    seek_off = lseek(fd, 10, SEEK_CUR);
    printf("lseek(fd, 10, SEEK_CUR) = %ld\n", seek_off);

    seek_off = lseek(fd, -50, SEEK_CUR);
    printf("lseek(fd, -50, SEEK_CUR) = %ld\n", seek_off);

    seek_off = lseek(fd, 1000000, SEEK_DATA);
    printf("lseek(fd, 1000000, SEEK_DATA) = %ld\n", seek_off);

    seek_off = lseek(fd, 100, SEEK_DATA);
    printf("lseek(fd, 100, SEEK_DATA) = %ld\n", seek_off);

    seek_off = lseek(fd, 0, SEEK_DATA);
    printf("lseek(fd, 0, SEEK_DATA) = %ld\n", seek_off);

    char buf[20];
    ssize_t nr = read(fd, buf, 20);
    printf("read(fd, buf, 20) = %ld\n", nr);

    // This line will print out some garbage value at the end.
    // Because the char buffer buf dose not end with specify '/0'.
    printf("%s\n", buf);

    return EXIT_SUCCESS;
}