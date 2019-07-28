#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    char *content = "hello world!\n";

    int fd_creat, fd_open, fd_dup;

    if ((fd_creat = creat("tmp3", 0644)) == -1) {
        perror("create");
        exit(EXIT_FAILURE);
    }
    close(fd_creat);

    if ((fd_open = open("tmp3", O_WRONLY | O_CREAT, 0644)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    fd_dup = fcntl(fd_open, F_DUPFD);

    printf("fd_open = %d, fd_dup = %d\n", fd_open, fd_dup);
    printf("write %lu bytes to fd_open\n", write(fd_open, content, strlen(content)));
    printf("write %lu bytes to fd_dup\n", write(fd_dup, content, strlen(content)));
    
    close(fd_open);
    close(fd_dup);
    
    printf("now the content of file:\n");
    system("cat tmp3");

    /*
     * The content of file should be:
     * Hello world!
     * Hello world!
     */

    return 0;
}