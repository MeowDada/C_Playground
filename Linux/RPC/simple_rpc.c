#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "serialize.h"
#define SERVER_IP "localhost"
#define SERVER_PORT 12345

static void serialize_int_wrapper(ser_buff_t buf , any_t dump)
{
    int val = (int)dump;
    serialize_int(buf, dump);
}

static any_t deserialize_int_wrapper(ser_buff_t buf, any_t dump)
{
    int temp = deserialize_int(buf);
    return temp;
}

void rpc_send_recv(ser_buff_t client_send_ser_buffer, ser_buff_t client_recv_ser_buffer)
{
    struct sockaddr_in dest;
    int sockfd = 0, rc = 0, recv_size = 0;
    int addr_len;

    dest.sin_family = AF_INET;
    dest.sin_port = htons(SERVER_PORT);
    struct hostent *host = (struct hostent *)gethostbyname(SERVER_IP);
    dest.sin_addr = *((struct in_addr *)host->h_addr);
    addr_len = sizeof(struct sockaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1) {
        printf("socket creation failed\n");
        return -1;
    }

    rc = sendto(sockfd,
                serialized_buffer_start_pos(client_send_ser_buffer),
                serialized_buffer_size(client_send_ser_buffer),
                0,
                (struct sockaddr *)&dest, sizeof(struct sockaddr));
    printf("%s() : %d bytes sent\n", __FUNCTION__, rc);
}

ser_buff_t multiply_client_stub_marshal(int a, int b)
{
    ser_buff_t client_send_ser_buff = serialized_buffer_create(NULL, NULL);
    serialize_int(client_send_ser_buff, a);
    serialize_int(client_send_ser_buff, b);
    return client_send_ser_buff;
}

int multiply_rpc(int a, int b)
{
    ser_buff_t *client_send_ser_buffer = multiply_client_stub_marshal(a, b);
    ser_buff_t *client_recv_ser_buffer = serialized_buffer_create(NULL, NULL);

    rpc_send_recv(client_send_ser_buffer, client_recv_ser_buffer);

    serialized_buffer_destroy(client_send_ser_buffer);
    client_send_ser_buffer = NULL;
}

int main(int argc, char **argv)
{
    int a = 3, b = 5;
    printf("multiply(%d,%d) = %d\n", a, b, multiply_rpc(a,b));

    return 0;
}