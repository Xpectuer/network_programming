// UDP Server
#include "common.h"

static int count = 0;

static void recvfrom_int(int signo)
{
    printf("\nreceived %d datagrams\n", count);
    exit(EXIT_SUCCESS);
}

// int main(int argc, char **argv) {
int udp_server(char* address, int port)
{
    int socket_fd;
    // create a datagram socket
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(strcmp(address, "0.0.0.0")==0)
    {
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else 
    {
        inet_pton(AF_INET, address, &server_addr.sin_addr);
    }
    
    server_addr.sin_port = htons(port);

    int on = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    signal(SIGINT, recvfrom_int);

    return socket_fd;
}
