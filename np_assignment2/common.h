#ifndef ALEX_COMMON_H
#define ALEX_COMMON_H

#include <linux/version.h>
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,5,44)
#error kernel version too low, update your kernel and remake 
#endif


#define EPOLL_ENABLE
#define LISTENQ 1024
#define MAXLINE 1024
#define MAX_INIT_SIZE 128
#define MAX_POLL_SIZE 128
#define TIMEOUT_INTERVAL 10
//#define DEBUG
//#define INFO

#define MSG_RCV 0
#define PRO_SET 1
#define PRO_RCV 2
#define MSG_END 3
#define TIMEOUT 4

#define TO_THRESHOLD 10

#define PROTOCOL_MAJOR_VER 1
#define PROTOCOL_MINOR_VER 0

#include "protocol.h"


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

#include <math.h>
#include <sys/select.h> /* for convenience */
// #include    <sys/sysctl.h>
#include <poll.h>    /* for convenience */
#include <strings.h> /* for convenience */
#include <sys/ioctl.h>
#include <pthread.h>

#ifdef EPOLL_ENABLE
#include <sys/epoll.h>
#endif

struct result_t
{
	int iresult;
	double fresult;

};

// for udp server to check if is down;
typedef struct context_t {
	int epfd;
	int listen_fd;
	// caches 
	uint8_t *state;
	uint8_t *mail_box;
	struct result_t *results;
	struct sockaddr_in *addrs;
	// load 
	uint32_t load;
	// timeout
	uint8_t *timeout; 	
} context_t;


struct p_client_addr
{
    struct sockaddr_in client_addr;
    uint16_t down;
};

#ifdef __cplusplus
extern "C"
{
#endif
    // listen and bind
    // returns a binded socket file descriptor
    int tcp_listen(char *address, int port);

    // returns a connected socket file descriptor
    int tcp_client(char *address, int port);

    // returns a UDP socket file descriptor
    int udp_server(char *address, int port);

    // fatal log the error and exit with failure
    void fatal(char *s);
    void print_bytes(size_t size, char *buf);
	
	// utilities for parsing protocol
	
	double ntohlf(double x);
	void parse_protocol(struct calcProtocol *response);
	void parse_msg(struct calcMessage *msg);
	void on_client_timeout(int fd, context_t * server);
	

	#define NA 0
	#define OK 1 
	#define NOK 2
	int send_msg(struct calcMessage *msg, int fd, struct sockaddr_in *client_addr);

	int recv_msg(struct calcMessage *msg, int fd, struct sockaddr_in *client_addr);


	int send_protocol(struct calcProtocol *prtcl, int fd, struct sockaddr_in *client_addr);
		

	int recv_protocol(struct calcProtocol *prtcl, int fd, struct sockaddr_in *client_addr);

	int gen_puzzle(struct calcProtocol* protocol, int* iresult, double *fresult);


	void make_nonblocking(int fd);

	int udp_nonblocking_listen(char *address, int port, struct context_t *ctx);


	int udp_accept(int listen_fd, struct sockaddr_in server_addr,struct sockaddr_in *addrs);
	
	int handler(int fd, context_t *server);

	void disconnect(int fd);
#ifdef __cplusplus
}
#endif

#endif // ALEX_COMMON_H
