#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
    if(argc != 2) {
        error(1,0,"usage: select01 <IPaddress>");
    }
    // first console arg: set socket ip addr
    int socket_fd = tcp_client(argv[1], SERV_PORT);
    
    char recv_line[MAXLINE], send_line[MAXLINE];
    int n;
    
    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(socket_fd, &allreads);
    
    while(true) {
        readmask = allreads;
        // 设置基数：可以减少内核的遍历次数
        // 需要+1，因为相当于数组的size定义，而数组下标从0开始
        // 超出0-1023的范围导致未定义的行为，因为select最大为1-1023的范围导致024
        int rc = select(socket_fd + 1, &readmask, NULL,NULL, NULL);
        if(rc <= 0) 
            error(1, errno, "select failed");
        
        if(FD_ISSET(socket_fd, &readmask)) {
            n = read(socket_fd, recv_line, MAXLINE);
            if(n<0) {
                error(1, errno, "read error");
            } else if(n == 0) {
                error(1, 0, "server terminated\n");
            }
            recv_line[n] = 0;
            fputs(recv_line, stdout);
            fputs("\n",stdout);
        }
        if(FD_ISSET(STDIN_FILENO, &readmask)) {
            if(fgets(send_line, MAXLINE, stdin) != NULL) {
                int i = strlen(send_line);
                if (send_line[i - 1] == '\n') {
                    send_line[i-1] = 0;
                }
                
                printf("now sending %s\n", send_line);
                size_t rt = write(socket_fd, send_line, strlen(send_line));
                if (rt < 0) {
                    error(1, errno, "write failed");
                }
                printf("send bytes: %zu \n", rt);
            }
        }
    }
}
