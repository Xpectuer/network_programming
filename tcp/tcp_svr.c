#include "lib/common.h"


#define BUF_SIZE 1024
void read_data(int sockfd) {
    ssize_t n;
    char buf[BUF_SIZE];

    int time = 0;
    for (;;) {
        fprintf(stdout, "block in read\n");
        if ((n = readn(sockfd, buf, BUF_SIZE)) == 0)
            return;
       
         
        time++;
        fprintf(stdout, "1K read for %d \n", time);
        usleep(1000);
    }
}


int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stdout, "usage: ./<executable> <port>\n");
        return EXIT_FAILURE;
    }
    char* char_port =  argv[1];
    uint16_t port = atoi(char_port);
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
    perror("bind error ! ");
    exit(EXIT_FAILURE);
    }
    // listen backlog set for 1024
    listen(listenfd, 1024);

    // accept client connections
    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        read_data(connfd);  // read data from "client's" connection fd
        close(connfd);      
    }
}

