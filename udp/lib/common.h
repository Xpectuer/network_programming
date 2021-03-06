#ifndef NOOBI_COMMON_H
#define NOOBI_COMMON_H

//#include "config.h"
//
// #include "log.h"
#include "tcp_server.h"
#include "udp_server.h"
//#include "inetaddress.h"
//#include "channel_map.h"

#include <sys/types.h>  /* basic system data types */
#include <sys/socket.h> /* basic socket definitions */
#include <sys/time.h>   /* timeval{} for select() */
#include <time.h>       /* timespec{} for pselect() */
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>  /* inet(3) functions */
#include <errno.h>
#include <fcntl.h> /* for nonblocking */
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> /* for S_xxx file mode constants */
#include <sys/uio.h>  /* for iovec{} and readv/writev */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h> /* for Unix domain sockets */

#include <sys/select.h> /* for convenience */
// #include    <sys/sysctl.h>
#include <poll.h>    /* for convenience */
#include <strings.h> /* for convenience */
#include <sys/ioctl.h>
#include <pthread.h>

#ifdef EPOLL_ENABLE
#include <sys/epoll.h>
#endif

size_t readn(int fd, void *vptr, size_t n);

void error(int status, int err, char *fmt, ...);

void info(char *fmt, ...);

void cmd_info(char *fmt, ...);

int tcp_client(char *ip_addr, int port);

// basic udp client for testing,
// netcat is recommended
int udp_client(char *ip_addr, int port);

#endif // NOOBI_COMMON_H
