#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

/* You will to add includes here */
#include "common.h"
// Included to get the support library
#include <calcLib.h>
#include "protocol.h"

#define CHILD 0

uint16_t major_version = 1;
uint16_t minor_version = 0; 
uint16_t down = 0;

static double ntohlf(double x)
{
	int n = 1;
	// little endian if true
	char* f = (char*) &x;
	int tmp;
	if(*(char *)&n == 1) 
	{	
		for(int i = 0; i < 4;i++)
		{
			f[i] = tmp;f[i]=f[7-i];f[i]=f[7-i];
		}	
	}
	return x;
}


void parse_protocol(struct calcProtocol *response)
{
	response->type=ntohs(response->type);
	response->major_version=ntohs(response->major_version);		
	response->minor_version=ntohs(response->minor_version);
	response->id=ntohl(response->id);
	response->arith=ntohl(response->arith);
	response->inValue1=(uint32_t)ntohl(response->inValue1);
	response->inValue2=(uint32_t)ntohl(response->inValue2);
	response->inResult=(uint32_t)ntohl(response->inResult);
	response->flValue1=ntohlf(response->flValue1);
	response->flValue2=ntohlf(response->flValue2);
	response->flResult=ntohlf(response->flResult);
}

void parse_msg(struct calcMessage *msg)
{
	msg->type=ntohs(msg->type);
	msg->message=ntohl(msg->message);
	msg->protocol=ntohs(msg->protocol);
	msg->protocol=ntohs(msg->major_version);
	msg->protocol=ntohs(msg->minor_version);

}

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

// *enhencement of read call
// auto set 0 for buffer
// check for error
// auto set the last byte to 0.
// [5.1]set timeout handler
// set non-blocking enhencement
int recvfrom_plus(int fd, void* buf, size_t size, struct sockaddr_in *client_addr, socklen_t *client_len)
{
	bzero(buf,size);	
	int n = recvfrom(fd, buf, size, 0, (struct sockaddr *)client_addr, client_len);
	if(n < 0)
	{
		
	    if(errno == EAGAIN || errno == EWOULDBLOCK)
		{
			puts("read timeout\n");
			on_client_timeout(fd, (struct sockaddr_in *)client_addr, *client_len);
			return -1;
		}
			
		else
		{
			fatal("read error... Exit"); 
		}
	}
	else if(n==0)
	{
		puts("client closed...");
		close(fd);
		return 0;
	}
	
	#ifdef DEBUG
	char *cli_addr = inet_ntoa(client_addr->sin_addr);
	uint16_t pport = client_addr->sin_port;
	printf("[info] connection established from %s %hu\n", cli_addr, pport);
	#endif
	
	
	//((char*)buf)[n] = 0;
	return n;
}

void handler()
{


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
	int fd = udp_server(address, port);
		// ------------------
		// for checking is down	
		
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr); 

		printf("%s:%d\n", address, port);
		struct calcMessage *msg = (struct calcMessage *)malloc(sizeof(struct calcMessage));
		struct calcProtocol * protocol= (struct calcProtocol *)malloc(sizeof(struct calcProtocol));
				
		// 1. receive first packet / validation
		int n = recvfrom(fd, (struct calcMessage*) msg, sizeof(struct calcMessage), 0, (struct sockaddr *) &client_addr, &client_len);		
		if(n == -1)
		{
			
			fatal("recvfrom 1");
		}
		parse_msg(msg);
		// version validation
		send_err(fd, msg, );
		
		
		
	


	
	printf("done.\n");
	return (0);
 }
