// UDP Server

#include "common.h"

#define SERVER_PORT 9090
#define MAXLINE 1024


static int count;

static void recvfrom_int(int signo) {
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}

int main(int argc, char **argv) {
    if(argc !=2)
		error(FATAL,0)
	int socket_fd;
    // create a datagram socket
    socket_fd = udp_server()
    struct sockaddr_in client_addr;

    client_len = sizeof(client_addr);
    for(;;) {
        int n = recvfrom(socket_fd, message, MAXLINE, 0, 
                        (struct sockaddr *) &client_addr, &client_len);

        message[n] = 0;
        printf("received %d bytes: %s\n", n, message);

        char send_line[MAXLINE];
        sprintf(send_line,"Hi, %s",message);

        sendto(socket_fd, send_line, 
                strlen(send_line),0,(struct sockaddr *) &client_addr, client_len);

        count++;
    }
}
