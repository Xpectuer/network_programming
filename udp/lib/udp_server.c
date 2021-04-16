// UDP Server
#include "udp_server.h"

static void recvfrom_int(int signo) {
    printf("\nreceived %d datagrams\n", count);
    exit(EXIT_SUCCESS);
}

// int main(int argc, char **argv) {
int udp_server(int port) 
{
    int socket_fd;
    // create a datagram socket
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
	
	int on = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bind(socket_fd,(struct sockaddr *) &server_addr, sizeof(server_addr));

   // socklen_t client_len;
   // char message[MAXLINE];
   // count = 0;

    signal(SIGINT, recvfrom_int);
	
	return socket_fd;
}




