#include "lib/common.h"

#define MAXLINE 1024

int main(int argc, char** argv) {
    if(argc != 3) {
        error(1,0,"usage: tcp_cli <ipaddress> <port>");
    }
    int port = atoi(argv[2]);
    int socket_fd = tcp_client(argv[1], port);
    
    // define buffers and state enumarations
    char recv_buf[MAXLINE], send_buf[MAXLINE];
    int rs;

    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(socket_fd, &allreads);
    
    while(1) {
        
        readmask = allreads;
        int rc = select(socket_fd + 1, &readmask, NULL,NULL,NULL);
        if(rc <=0) 
          error(1,errno,"select failed!");
        
        // read event: print msg to stdout
        if (FD_ISSET(socket_fd, &readmask)) {
            // syscall read
            n = read(socket_fd, recv_buf, MAXLINE);
            if(n < 0) { // error occured
                error(1,errno, "read error");
            } else if(n == 0) { // closed
                printf("server closed\n");
                break;
            }
        }  
        if (FD_ISSET(stdin, &readmask)) {
            if(fgets(send_buf, MAXLINE, stdin) != NULL) {
                int i = strlen(send_line);
                if(send_buf[i-1] == '\n') {
                    send_buf[i-1] = '\0';         
                }
                // exit cmd and gracefully shutdown
                if(strcmp(send_buf,"exit", strlen(send_buf)) ==0) {
                    if(shutdown(socket_fd,1)) {
                        error(1, errno, "shutdown failed");
                    }
                }

                size_t rt = write(socket_fd, send_buf, strlen(send_buf));
                if(rt < 0) {
                    error(1, errno, "write failed!");
                }
            }
        }
    } 

    return 0;
}
