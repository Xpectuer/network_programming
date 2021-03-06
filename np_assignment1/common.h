#ifndef ALEX_COMMON_H
#define ALEX_COMMON_H


#define LISTENQ 5
#define MAXLINE 1024
// 9-4 = 5
#define MAX_FD_Q 9

#include "calcLib.h"

#include    <sys/types.h>    /* basic system data types */
#include    <sys/socket.h>    /* basic socket definitions */
#include    <sys/time.h>    /* timeval{} for select() */
#include    <time.h>        /* timespec{} for pselect() */
#include    <netinet/in.h>    /* sockaddr_in{} and other Internet defns */
#include    <arpa/inet.h>    /* inet(3) functions */
#include    <errno.h>
#include    <fcntl.h>        /* for nonblocking */
#include    <netdb.h>
#include    <signal.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/stat.h>    /* for S_xxx file mode constants */
#include    <sys/uio.h>        /* for iovec{} and readv/writev */
#include    <unistd.h>
#include    <sys/wait.h>
#include    <sys/un.h>        /* for Unix domain sockets */
//#include 	<sys/sem.h>
#include    <sys/select.h>    /* for convenience */
// #include    <sys/sysctl.h>
#include	<semaphore.h>

#include    <poll.h>        /* for convenience */
#include    <strings.h>        /* for convenience */
#include    <sys/ioctl.h>
#include    <pthread.h>
#include <math.h>


#ifdef EPOLL_ENABLE
#include    <sys/epoll.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// listen and bind  
// returns a binded socket file descriptor
int tcp_listen(char* address, int port);

// returns a connected socket file descriptor
int tcp_client(char *address, int port);


// fatal log the error and exit with failure
void fatal(char* s);


void handler(int fd);
#ifdef __cplusplus
}
#endif

#endif // ALEX_COMMON_H
