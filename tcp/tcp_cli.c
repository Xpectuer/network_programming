#include "lib/common.h"

#define MSG_SIZE 102400

void send_data(int sockfd) {
    char* query; 
    query = malloc(MSG_SIZE + 1);
   // for(int i = 0; i < MSG_SIZE; i++) {
   //     query[i] = 'a';
   // }
    
    fgets(query,MSG_SIZE, stdin);
    // query[MSG_SIZE] = '\0';
    // point to constant char
    const char* cp;
    cp = query;
    size_t remaining = strlen(query);
    while(remaining) {
        int n_written = send(sockfd, cp, remaining, 0);
        fprintf(stdout,"send %d bytes to the server",n_written);
        if(n_written <=0) {
            error(1, errno, "send failed!");
            return;        
        }
        remaining -= n_written;
        cp += n_written;
    }
    return;
}

int main(int argc, char** argv) {

    int socket_fd;
    struct sockaddr_in svr_addr;
    char* ip;
    int  port;
    ip = argv[1];
    port = atoi(argv[2]);

    if(argc != 3)
        error(1,0,"usage: tcp client <IP addr> <Port>");

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
       
    bzero(&svr_addr, sizeof(svr_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(port);

    // puts("123");
    inet_pton(AF_INET, ip, &svr_addr.sin_addr);

    
    int connect_rt = connect(socket_fd, (struct sockaddr *) &svr_addr, sizeof(svr_addr)); 

    if(connect_rt < 0) {
        error(1, errno, "connection failed!");
    }
    while(1)
        send_data(socket_fd);
    
    return EXIT_SUCCESS;
}
