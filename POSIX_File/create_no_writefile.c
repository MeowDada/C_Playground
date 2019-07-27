#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd;
    const char *content = "test message!\n";

    fd = open(argv[1], O_CREAT | O_WRONLY, 0000);
    write(fd, content, strlen(content)+1);
    close(fd);

    return 0;
}