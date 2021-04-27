#include <stdio.h>
#include <stdlib.h>
/* You will to add includes here */
#include "common.h"

// Included to get the support library
#include <calcLib.h>


#include "protocol.h"

void usage()
{
    puts("usage: ./client <ip:port>");
    exit(1);
}

void init()
{
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
}

//char read_byte(char** s)
//uint16_t read_uint16(char** s)
//uint32_t read_uint32(char** s)
//int32_t read_int32(char** s)
//double read_lf(char** s)


void parse_protocol(char* r, struct calcProtocol *response, size_t n)
{
	char** s = &r;
	response->type=ntohs(read_uint16(s));
	response->major_version=ntohs(read_uint16(s));		
	response->minor_version=ntohs(read_uint16(s));
	response->id=ntohl(read_uint32(s));
	response->arith=ntohl(read_uint32(s));
	response->inValue1=(uint32_t)ntohl(read_int32(s));
	response->inValue2=(uint32_t)ntohl(read_int32(s));
	response->inResult=(uint32_t)ntohl(read_int32(s));
	response->flValue1=read_lf(s);
	response->flValue2=read_lf(s);
	response->flResult=read_lf(s);
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
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;
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
    char send_buf[MAXLINE], recv_buf[MAXLINE+1];
    socklen_t len;

	// init protocol
	struct calcMessage *msg;
    size_t	msg_len = sizeof(struct calcMessage);
	msg = (struct calcMessage* )malloc(sizeof(struct calcMessage));
	msg->type=htons(22);
	msg->message=htons(0);
	msg->protocol=htons(17);
	msg->major_version=htons(1);
	msg->minor_version=htons(0);	

	#ifdef DEBUG
	puts("send to");
	#endif	
	// sendto 	
	struct calcProtocol* response;
	response = (struct calcProtocol*) malloc(sizeof(struct calcProtocol));

	for(;;)
	{
		size_t rt = sendto(fd,
	                      msg, msg_len, 
	                       0,(struct sockaddr *)&server_addr,
						   server_len);
		if(rt < 0)
		{
			fatal("Unable to send msg, Exit.");		
		}
	
		len = 0;	
		printf("send %d bytes..\n",sizeof(struct calcProtocol));
	
			
		rt = recvfrom(fd, (struct calcProtocol*) response, sizeof(struct calcProtocol), 0, reply_addr, &len);
		//rt = read(fd, recv_buf, sizeof(recv_buf));
		if(rt ==-1)
		{
			puts("recv timeout, resending...");
			//fatal("Unable to receive msg, Exit.");
			continue;
		}
		if(rt > 0)
		{
	
			printf("recv %d bytes,type is:%d\n",rt, ntohs(response->type));	
			break;
		}
	}
	free(response);
	free(msg);
	
}
