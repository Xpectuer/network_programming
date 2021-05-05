#include "common.h"
void make_nonblocking(int fd)
{
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

int udp_accept(int listen_fd, struct sockaddr_in server_addr,struct sockaddr_in *addrs)
{

	int ret;
	int conn_fd;
	int reuse = 1;
	struct sockaddr_in client_addr;
	socklen_t          client_len = sizeof(client_addr);
	struct calcMessage  message; 	
		
	ret = recvfrom(listen_fd, &message, sizeof(message), 0, (struct sockaddr *)&client_addr, &client_len);
	if(ret == -1)
	{
		perror("recvfrom");
		exit(1);
	}	
	// TODO: not working
	parse_msg(&message);
	
	if((conn_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket");
		fatal("socket");
	}
	#ifdef DEBUG
	printf("listen fd: %d; connection fd: %d.\n", listen_fd, conn_fd);
	
	printf("%d \n", ntohs(message.major_version));
	#endif
	if(ntohs(message.major_version) != PROTOCOL_MAJOR_VER  || ntohs(message.minor_version) != PROTOCOL_MINOR_VER )
	{				
		ret = sendto(listen_fd, &message, sizeof(message), 0, (struct sockaddr *)&client_addr, client_len);
		if(ret == -1)
		{
			perror("sendto");
			exit(1);
		}
#ifdef INFO
		printf("%d bytes sent\n", ret);
#endif
		return -1;
	}
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
	
	memcpy(&addrs[conn_fd],&client_addr,sizeof(struct sockaddr_in));
	return conn_fd;
}
