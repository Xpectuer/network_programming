#include "common.h"


int tcp_client(char *address, int port) {    
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    if(address == NULL) {
       //error(1,0,"Invalid Address");
        puts("Invalid Address");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, address, &server_addr.sin_addr);

    socklen_t server_len = sizeof(server_addr);
    int connect_rt = connect(socket_fd, (struct sockaddr *) &server_addr, server_len);
    if (connect_rt < 0) {
        //error(1, errno, "connect failed ");
        puts("connect failed");
        exit(1);
    }

    return socket_fd;
}
