#include "lib/common.h"

int count = 0;

static void telnet_handler(int connfd)
{
	char buf[256];
	while (1)
	{
		bzero(buf, sizeof(buf));
		int n = read(connfd, buf, sizeof(buf));

		if (n < 0)
		{
			error(1, errno, buf, sizeof(buf));
		}
		else if (n == 0)
		{
			info( "client closed.");
			close(connfd);
			break;
		}
		count += n * sizeof(char);
		//count++;
		buf[n] = 0;

		info("%s\n",buf);
		// if(strncmp(buf,"quit",4)==0)
		// {	
		// 	close(connfd);
		// 	break;
		// }
		char *result = malloc(256 * sizeof(char));
		// TODO: bug
		sprintf(result, "Hello, %s\n", buf);

		if (send(connfd, result, strlen(result), 0) < 0)
			error(1, 0, "send failed");

		free(result);
	}
}
void init()
{
	setbuf(stdin, 0);
	setbuf(stdout, 0);
	setbuf(stderr, 0);
}

int main(int argc, char **argv)
{
	init();
	if (argc != 2)
		error(1, 0, "usage: ./svr <port>");

	int port = atoi(argv[1]);
	int listen_fd;

	listen_fd = tcp_listen(port);

	int conn_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	while (1)
	{	
		
		if ((conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len)) < 0)
		{
			error(1, errno, "bind failed ");
		}
		//printf("%d\n", conn_fd);
		//sleep(1);
		
		//	printf("[INFO] connection established from %s %hu\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
		version();				

		telnet_handler(conn_fd);
		info("%d bytes received", count);
		count = 0;
	}
	return 0;
}
