
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

/* You will to add includes here */
#include "common.h"
// Included to get the support library
//#include <calcLib.h>


// context of server


context_t *server;

void disconnect(int fd)
{
	// over the sessfdon	
	server->state[fd] = MSG_RCV;	
	server->timeout[fd] = 0;
	server->mail_box[fd]=0;
	server->results[fd].iresult=0;
	server->results[fd].fresult=0.0;
	epoll_ctl(server->epfd, EPOLL_CTL_DEL, fd, NULL);
	// TODO: may be bug
	close(fd);	

}

void timer_runner(void* fdp)
{
	sleep(TIMEOUT_INTERVAL);
	// global context
	puts("www");
	int fd= *(int*) fdp;
#ifdef DEBUG
	printf("server:%p fd:%d\n ",server,fd);
#endif
	server->timeout[fd] = 1;	
	free(fdp);
}

void start_timer(int fd)
{	
	pthread_t tid;	
	int *fdp = malloc(sizeof(int));
	*fdp = fd;
	pthread_create(&tid,NULL,timer_runner, (void *)fdp);
}


void print_banner()
{
	puts("   ______                             __          ____   __");
	puts("  / ____/  ____     ____    __  __   / /  ___    / __/  / /_");
	puts(" / /      / __ \\   / __ \\  / / / /  / /  / _ \\  / /_   / __/");
	puts("/ /___   / /_/ /  / /_/ / / /_/ /  / /  /  __/ / __/  / /_   _");
	puts("\\____/   \\____/  / .___/  \\__, /  /_/   \\___/ /_/     \\__/  (_)");
	puts("                /_/      /____/");
	puts("    ___     __     ______   _  __");
	puts("   /   |   / /    / ____/  | |/ /");
	puts("  / /| |  / /    / __/     |   /");
	puts(" / ___ | / /___ / /___    /   |");
	puts("/_/  |_|/_____//_____/   /_/|_|");
	puts("");
	puts("    ______                    __    __          ______            __                   __           __");
	puts("   / ____/    ____   ____    / /   / /         / ____/  ____ _   / /  _____  __  __   / /  ____ _  / /_  ____    _____");
	puts("  / __/      / __ \\ / __ \\  / /   / /         / /      / __ `/  / /  / ___/ / / / /  / /  / __ `/ / __/ / __ \\  / ___/");
	puts(" / /___     / /_/ // /_/ / / /   / /         / /___   / /_/ /  / /  / /__  / /_/ /  / /  / /_/ / / /_  / /_/ / / /");
	puts("/_____/    / .___/ \\____/ /_/   /_/          \\____/   \\__,_/  /_/   \\___/  \\__,_/  /_/   \\__,_/  \\__/  \\____/ /_/");
	puts("          /_/                                                                                                       ");
}

void init()
{
	setbuf(stdout, 0);
	setbuf(stderr, 0);
	setbuf(stdin, 0);
}

int main(int argc, char *argv[])
{

	init();
	print_banner();
	/* Do more magic */
	if(argc != 2) fatal("usage: ./server <ip>:<port>");	
	/* 
	   Prepare to setup a reoccurring event every 10s. If it_interval, or it_value is omitted, it will be a single alarm 10s after it has been set. 
	*/
			
	char *arg = argv[1];
	
	char sign[] = ":";
	char *address = strtok(arg, sign);
	int port = atoi(strtok(NULL, sign));	
	// ---init of server---
	server = malloc(sizeof(struct context_t));
	uint8_t *state = (server->state=calloc(MAX_INIT_SIZE,sizeof(uint8_t)));
	struct sockaddr_in *addrs = (server->addrs = calloc(MAX_INIT_SIZE,sizeof(struct sockaddr_in)));
	int listen_fd = (server->listen_fd = udp_nonblocking_listen(address, port,server));
	struct sockaddr_in server_addr = server->addrs[listen_fd];
	server->load = 1;
	int epfd = (server->epfd = epoll_create(MAX_POLL_SIZE)); 
	uint8_t *mail_box = (server->mail_box  = malloc(MAX_INIT_SIZE)); 
	struct result_t *results = (server->results = calloc(MAX_INIT_SIZE, sizeof(struct result_t)));
	uint8_t *timeout = (server->timeout = calloc(MAX_INIT_SIZE, sizeof(uint8_t)));
	// ---
	
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

	int check_interval = 200;

	for(;;)
	{
		// max event return 999 ,200ms timeout(1 times check/s)
		nfds = epoll_wait(epfd, events, 999, -1);	
#ifdef DEBUG
		//printf("nfds:%d\n",nfds);
#endif
		// TODO: check for timeout array
		// except for listen fd
		if(nfds >= 0)
		{
			for(int i=0; i < nfds; i++)
			{	
				int fd = events[i].data.fd;
				// accept
				if(fd == listen_fd)
				{
#ifdef DEBUG
					printf("listen fd: %d\n", listen_fd);
#endif	
				
					struct epoll_event conn_ev;
				
					int conn_fd = udp_accept(listen_fd, server_addr, addrs);
					if(conn_fd == -1)
					{
						continue;	
					}
					conn_ev.data.fd = conn_fd;
					conn_ev.events = EPOLLOUT | EPOLLIN;	 
					state[conn_fd] = MSG_RCV;
					server->timeout[conn_fd] = 0; 
					
					// 10s	
					//start_timer(conn_fd, server);
					make_nonblocking(conn_fd);
					printf("%d\n",conn_fd);
					start_timer(conn_fd);
					if(epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &conn_ev) < 0)
					{
						//TODO: clean inactive fds
						fprintf(stderr, "unable to add event for conn fd: %d\n", conn_fd);
						return -1;
					} 
					printf("added fd: %d\n",conn_fd);	
					server->load++;		
				}
				else
				{
#ifdef DEBUG
					printf("fd:%d\n",fd);		
#endif
					// callback	
					if(handler(fd, server) == -1)
					{	

						send_timeout(fd, server);
						disconnect(fd);
					}
						
				}	
			}	
		}
		else if(nfds < 0)
		{
			perror("epoll wait");
			exit(1);		
		}
	

		
	}
#ifdef DEBUG
#endif	
//	free(state);
//	free(addrs);
//	free(server);
	close(listen_fd);
	return (0);
 }
