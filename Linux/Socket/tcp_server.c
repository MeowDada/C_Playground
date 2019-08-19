#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include "logger.h"

#define MAX 80 
#define PORT 8080 

static void do_stuff(int sockfd)
{
    char buf[MAX];
    int n = 0;

    while (1) {
        bzero(buf, MAX);

        read(sockfd, buf, sizeof(buf));
        LOGGING_INFO("from client: %s\t to client : ", buf);
        bzero(buf, MAX);
        n = 0;

        while((buf[n++] = getchar()) != '\n');

        write(sockfd, buf, sizeof(buf));

        if (strncmp("exit", buf, 4) == 0) {
            LOGGING_INFO("server exit...");
            break;
        }
    }
}

int main(int argc, char **argv)
{
    setup_logger(NULL, LOG_LEVEL_INFO);

    int sockfd, connfd, len;

    struct sockaddr_in serv_addr, cli;

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
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(PORT);

    if ((bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) != 0) {
        LOGGING_ERROR("socket bind failed...");
        goto end;
    }
    else {
        LOGGING_INFO("socket bind successfully");
    }

    /* start listening */
    if ((listen(sockfd, 5)) != 0) {
        LOGGING_ERROR("listen failed...");
        goto end;
    }
    else {
        LOGGING_INFO("server listening...");
    }

    len = sizeof(cli);

    /* Accept the data packet from client and verification */
    connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
    if (connfd < 0) {
        LOGGING_ERROR("server accept failed...");
        goto end;
    }
    else {
        LOGGING_INFO("server accept the client...");
    }
    do_stuff(connfd);

    close(sockfd);

end:
    close_logger(NULL);

    return 0;
}