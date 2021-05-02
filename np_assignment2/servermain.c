#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

/* You will to add includes here */
#include "common.h"
// Included to get the support library
#include <calcLib.h>
//#include "protocol.h"

#define CHILD 0

uint16_t major_version = 1;
uint16_t minor_version = 0; 
uint16_t down = 0;



void on_client_timeout(int fd,struct sockaddr *client_addr, socklen_t addr_len)
{
	char* buf ="ERROR TO";
	sendto(fd, buf, strlen(buf),0, client_addr, addr_len);
	return;
}

// return:
// 1  float
// 0  integer
// -1 error

void handler(int fd)
{

}


int udp_accept(int listen_fd, struct sockaddr_in server_addr)
{

	int ret;
	int conn_fd;
	int reuse = 1;
	struct sockaddr_in client_addr;
	socklen_t          client_len = sizeof(client_addr);
	struct calcMessage  message; 	
 	struct calcProtocol protocol; 
	
	ret = recvfrom(listen_fd, &message, sizeof(message), 0, (struct sockaddr *)&client_addr, &client_len);
	if(ret == -1)
	{
		perror("recvfrom");
		exit(1);
	}	

	if((conn_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket");
		fatal("socket");
	}
	#ifdef DEBUG
	printf("listen fd: %d; connection fd: %d.\n", listen_fd, conn_fd);
	#endif

	ret = setsockopt(conn_fd, SOL_SOCKET, SO_REUSEADDR, &reuse,sizeof(reuse));
    if (ret == -1) {
        exit(1);
    }

    ret = setsockopt(conn_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));
    if (ret == -1) {
        exit(1);
    }
	ret = bind(conn_fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr));	
	if(ret == -1)
	{
		perror("conn bind");
		exit(1);
	}

	if(connect(conn_fd, (struct sockaddr *) &client_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect");
		exit(1);
	}


}

int main(int argc, char *argv[])
{

	/* Do more magic */
	if(argc != 2)	fatal("usage: ./server <ip>:<port>");	
	/* 
	   Prepare to setup a reoccurring event every 10s. If it_interval, or it_value is omitted, it will be a single alarm 10s after it has been set. 
	*/
			
	char *arg = argv[1];
	
	char sign[] = ":";
	char *address = strtok(arg, sign);
	int port = atoi(strtok(NULL, sign));	
	initCalcLib();	
	int ret;
 	int listen_fd;	
	int epfd;
    // create a datagram socket
    listen_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(strcmp(address, "0.0.0.0")==0)
    {
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else 
    {
        inet_pton(AF_INET, address, &server_addr.sin_addr);
    }
    
    server_addr.sin_port = htons(port);

    int reuse = 1;
    ret = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	if(ret == -1)
	{
		perror("setsockopt addr");
		exit(1);
	}

	ret = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse));
	if(ret == -1)
	{
		perror("setsockopt port");
		exit(1);
	}


   	if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind listen");
		exit(1);
	}

	epfd = epoll_create(MAX_POLL_SIZE); 
	
	struct epoll_event ev;
    struct epoll_event events[MAX_POLL_SIZE];

	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = listen_fd;

	if(epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) < 0)
	{
		fprintf(stderr, "unable to add event for fd: %d\n", listen_fd);
		return -1;
	} 
#ifdef DEBUG
	puts("add ok.");
#endif
	int nfds;
	for(;;)
	{
		// max event return 999 no timeout
		nfds = epoll_wait(epfd, events, 999, -1);	
		if(nfds == -1)
		{
			perror("epoll_wait");
			break;
		}
		
		for(int i=0; i < nfds; i++)
		{	
			// accept
			if(events[i].data.fd == listen_fd)
			{

#ifdef DEBUG
				printf("listen fd: %d\n", listen_fd);
#endif	
				struct epoll_event conn_ev;
				int conn_fd = udp_accept(listen_fd, server_addr);
				if(epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &conn_ev) < 0)
				{
					fprintf(stderr, "unable to add event for conn fd: %d\n", conn_fd);
					return -1;
				} 
				else
				{
					handler(events[i].data.fd);
				}		  	
			}
		}	
	}
	close(listen_fd);
	return (0);
 }
