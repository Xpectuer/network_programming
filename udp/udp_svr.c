// UDP Server

#include "lib/common.h"


static int count;

static void recvfrom_int(int signo)
{
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        error(1, 0, "usage: ./svr <port>");

    int port = atoi(argv[1]);
    int socket_fd;
    // create a datagram socket
    socket_fd = udp_server(port);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char message[MAXLINE];
    for (;;)
    {
        int n = recvfrom(socket_fd, message, MAXLINE, 0,
                         (struct sockaddr *)&client_addr, &client_len);

        message[n] = 0;
        
        printf("received %d bytes: %s\n", n, message);

        char send_line[MAXLINE];
        sprintf(send_line, "Hi, %s", message);

        sendto(socket_fd, send_line,
               strlen(send_line), 0, (struct sockaddr *)&client_addr, client_len);

        count++;
    }
}
