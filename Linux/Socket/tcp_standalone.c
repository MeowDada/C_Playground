#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "common.h"

#define PORT 8080

typedef struct socket_instance {
    int fd;
} socket_instance;

void create_socket(int id, void *args)
{
    socket_instance *sock = (socket_instance *)args;

    sock->fd = socket(AF_INET, SOCK_STREAM, 0);
}

void socket_server(int id, void *args)
{
    socket_instance *sock = (socket_instance *)args;

    int server_fd = sock->fd;
    int client_fd = 0;
    int opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        &opt, sizeof(opt))) {
        LOGGING_ERROR("failed to set socket option");
        return;
    }

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(PORT);

    LOGGING_INFO("[SERVER]: start binding...");
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        LOGGING_ERROR("server socket failed to bind");
        return;
    }
    LOGGING_INFO("[SERVER]: binding succeed");

    LOGGING_INFO("[SERVER]: start listening...");
    if (listen(server_fd, 3) < 0) {
        LOGGING_ERROR("server socket failed to listen");
        return;
    }
    LOGGING_INFO("[SERVER]: listening succeed");

    LOGGING_INFO("[SERVER]: start accepting client...");
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address,
        (socklen_t *)&addrlen)) < 0) {
        LOGGING_ERROR("server socket failed to accept");
        return;
    }
    LOGGING_INFO("[SERVER]: accept client succeed");

    char buffer[1024];
    int val_read = read(client_fd, buffer, 1024);
    LOGGING_INFO("[SERVER]: read = %s", buffer);

    char greetings[] = "Hello from server";
    send(client_fd, greetings, strlen(greetings),0);
    LOGGING_INFO("[SERVER]: hello message sent");
}

void socket_client(int id, void *args)
{
    socket_instance *sock = (socket_instance *)args;

    int client_fd = sock->fd;
    struct sockaddr_in address;
    char connect_target_ip[] = "127.0.0.1"; 

    address.sin_family = AF_INET;
    address.sin_port   = htons(PORT);

    if (inet_pton(AF_INET, connect_target_ip, &address.sin_addr) <= 0) {
        LOGGING_ERROR("invalid address: address not supported");
        return;
    }

    LOGGING_INFO("[CLIENT]: client socket start connecting to %s", connect_target_ip);
    if (connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        LOGGING_ERROR("[CLIENT]: connection failed");
        return;
    }
    LOGGING_INFO("[CLIENT]: connection succeed");

    char greetings[] = "Hello from client";
    send(client_fd, greetings, strlen(greetings), 0);
    LOGGING_INFO("[CLIENT]: hello message sent");

    char buffer[1024];
    int valread = read(client_fd, buffer, 1024);
    LOGGING_INFO("[CLIENT]: read = %s", buffer);
}

int main(int argc, char **argv)
{
    setup_logger(NULL, LOG_LEVEL_INFO);

    socket_instance *sock_server   = (socket_instance *)malloc(sizeof(socket_instance));
    socket_instance *sock_client = (socket_instance *)malloc(sizeof(socket_instance));

    threadpool_t *server_thread = thpool_init(1);
    threadpool_t *client_thread = thpool_init(1);

    thpool_add_work(server_thread, create_socket, (void *)sock_server);
    thpool_add_work(client_thread, create_socket, (void *)sock_client);

    thpool_wait(server_thread);
    thpool_wait(client_thread);

    thpool_add_work(server_thread, socket_server, (void *)sock_server);
    thpool_add_work(client_thread, socket_client, (void *)sock_client);

    thpool_wait(server_thread);
    thpool_wait(client_thread);

    thpool_destroy(server_thread);
    thpool_destroy(client_thread);    

    close_logger(NULL);

    return 0;
}