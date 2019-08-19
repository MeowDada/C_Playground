#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h>
#include "logger.h"

#define MAX 80 
#define PORT 8080

static void do_stuff(int sockfd)
{
    char buf[MAX];
    int n = 0;
    while (1) {
        bzero(buf, sizeof(buf));
        LOGGING_INFO("Enter the string : ");
        n = 0;
        while ((buf[n++] = getchar()) != '\n');

        write(sockfd, buf, sizeof(buf));
        bzero(buff, sizeof(buf));
        read(sockfd, buf, sizeof(buf));
        LOGGING_INFO("from server : %s", buf);
        if ((strncmp(buf, "exit", 4)) == 0) {
            LOGGING_INFO("client exit...");
            break;
        }
    }
}

int main(int argc, char **argv)
{
    setup_logger(NULL, LOG_LEVEL_INFO);

    int sockfd, connfd, len;

    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        LOGGING_ERROR("socket creation failed...");
        goto end;
    }
    else {
        LOGGING_INFO("socket created successfully");
    }
    bzero(&serv_addr, sizeof(serv_addr));

    /* Assign IP and ports */
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl("127.0.0.1");
    serv_addr.sin_port        = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
        LOGGING_ERROR("connection with the server failed...");
        goto end;
    }
    else {
        LOGGING_INFO("connected to the server");
    }

    do_stuff(connfd);

    close(sockfd);
end:
    close_logger(NULL);

    return 0;
}