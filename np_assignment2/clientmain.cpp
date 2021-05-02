#include <stdio.h>
#include <stdlib.h>
/* You will to add includes here */
#include "common.h"

// Included to get the support library
#include <calcLib.h>
#include "protocol.h"


int major_version = 1;
int minor_version = 0;

void usage()
{
    puts("usage: ./client <ip:port>");
    exit(1);
}

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

void init()
{
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
}

void make_msg(calcMessage *msg)
{
	msg->type=htons(22);
	msg->message=htons(0);
	msg->protocol=htons(17);
	msg->major_version=htons(major_version);
	msg->minor_version=htons(minor_version);	
}

//void parse_protocol(char* r, struct calcProtocol *response, size_t n)
//{
//	char** s = &r;
//	response->type=ntohs(read_uint16(s));
//	response->major_version=ntohs(read_uint16(s));		
//	response->minor_version=ntohs(read_uint16(s));
//	response->id=ntohl(read_uint32(s));
//	response->arith=ntohl(read_uint32(s));
//	response->inValue1=(uint32_t)ntohl(read_int32(s));
//	response->inValue2=(uint32_t)ntohl(read_int32(s));
//	response->inResult=(uint32_t)ntohl(read_int32(s));
//	response->flValue1=read_lf(s);
//	response->flValue2=read_lf(s);
//	response->flResult=read_lf(s);
//}
//
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

void abort()
{
	fatal("Aborting...");
}

// opt
#define SEND 0
#define RECV 1
int protocol_ctl(int opt, struct calcProtocol *response)
{
	
}

int msg_ctl(int opt, struct calcMessage *msg)
{

}


inline int version_check(calcMessage *msg, calcProtocol *response)
{
	return (ntohs(msg->major_version)==ntohs(response->major_version))?0:1;
} 



int main(int argc, char *argv[]){
    init();
    /* Do magic */
    if(argc != 2) usage(); 

    char* sign = ":";
    char *address = strtok(argv[1], sign);
    int port = atoi(strtok(NULL, sign));
    
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
	//setbuf((FILE*)fd,0);
	
	struct timeval read_timeout;
	read_timeout.tv_sec = 2;
	read_timeout.tv_usec = 0;
	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
	// ---
    struct sockaddr_in server_addr;
    
    bzero(&server_addr, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, address, &server_addr.sin_addr);
    

    socklen_t server_len = sizeof(server_addr); 
    struct sockaddr *reply_addr;
    reply_addr =  (sockaddr*)malloc(server_len);
	
    // +1 in case of buffer overflow
    //char send_buf[MAXLINE], recv_buf[MAXLINE+1];
	char send_buf[MAXLINE];
    socklen_t len;

	// init protocol
	struct calcMessage *msg;
    size_t msg_len = sizeof(struct calcMessage);
	
	msg = (struct calcMessage* )malloc(sizeof(struct calcMessage));
	make_msg(msg);
	#ifdef DEBUG
	printf("message is: %d\n", msg->type);
	puts("send to");
	#endif	

	// sendto 	
	struct calcProtocol* response; 
	response = (struct calcProtocol*) malloc(sizeof(struct calcProtocol));
	for(;;)
	{
		size_t rt = sendto(fd,
	                       msg, msg_len, 
	                       0, (struct sockaddr *) &server_addr,
						   server_len);
		if(rt < 0)
		{
			fatal("Unable to send msg, Exit.");		
		}
	
		len = 0;	
		printf("send %d bytes..\n", sizeof(struct calcProtocol));
		
		rt = recvfrom(fd, (struct calcProtocol*) response, sizeof(struct calcProtocol), 0, reply_addr, &len);
		// resending
		if(rt ==-1)
		{
			puts("recv timeout, resending...");
			//fatal("Unable to receive msg, Exit.");
			continue;
		}
		break;
	}
	parse_protocol(response);
	printf("float is: %lf\n",response->flResult);	
	// version check
	
	char arith_sign[4]={
		'+',
		'-',
		'*',
		'/'
	};
	response->type++;	
	if(response->arith < 5)
	{	
		printf("%d %c %d \n",
				response->inValue1, 
				arith_sign[(response->arith-1)&3], 
				response->inValue2);		
		if(fgets(send_buf, MAXLINE, stdin) != NULL)
		{
			int i = strlen(send_buf); 	
			send_buf[i] = 0; 
		}
		response->inResult = (int32_t) atoll(send_buf);	
			
	}
	// float opts
	else if(response->arith >= 5 && response->arith <=8)
	{
		
		printf("%8.8g %c %8.8g \n",
				response->flValue1, 
				arith_sign[(response->arith-1)&3], 
				response->flValue2);		
		if(fgets(send_buf, MAXLINE, stdin) != NULL)
		{
			int i = strlen(send_buf); 	
			send_buf[i] = 0; 
		}
	
		response->flResult = (double) strtod(send_buf, NULL);
	}	
	else 
	{	
		puts("Invalid Arith...Exit.");
		abort();	
	}
	
	for(;;)
	{
		int rt = sendto(fd,(struct calcProtocol *) response,sizeof(struct calcProtocol) , 
		            0, (struct sockaddr *) &server_addr, server_len);
		if(rt < 0)
		{
			fatal("Unable to send response...Exit");	
		}		
		#ifdef DEBUG
		printf("sent %d bytes to server\n",rt);
		#endif
		len = 0;
		int n = recvfrom(fd, (struct calcMessage *) msg, sizeof(struct calcMessage), 0, reply_addr, &len); 	
		if(rt == -1)
		{
			puts("recv timeout, resending...");
			//fatal("Unable to receive msg, Exit.");
			continue;
		}
		#ifdef DEBUG
		printf("received %d bytes from server\n", n);
		#endif	
		break;
	}		
	char* msg_sign[2]={
		"OK",
		"NOT OK"
	};
	parse_msg(msg);
	printf("%s\n", msg_sign[(msg->message-1)&1]);
		
	free(response);
	free(msg);
	
}
