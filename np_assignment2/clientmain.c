#include <stdio.h>
#include <stdlib.h>
/* You will to add includes here */
#include "common.h"

// Included to get the support library
#include <calcLib.h>
// #include "protocol.h"


int major_version = 1;
int minor_version = 0;

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

void make_msg(struct calcMessage *msg)
{
	msg->type=htons(22);
	msg->message=htons(0);
	msg->protocol=htons(17);
	msg->major_version=htons(major_version);
	msg->minor_version=htons(minor_version);	
}

void abort()
{
	fatal("Aborting...");
}

inline int version_check(struct calcMessage *msg, struct calcProtocol *response)
{
	return (ntohs(msg->major_version)==ntohs(response->major_version))?0:1;
} 



int main(int argc, char *argv[])
{
    init();
    /* Do magic */
    if(argc != 2) usage(); 

    char* sign = ":";
    char *address = strtok(argv[1], sign);
    int port = atoi(strtok(NULL, sign));
    
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
	//setbuf((FILE*)fd,0);
	
	struct timeval read_timeout;
	read_timeout.tv_sec = 10;
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
    reply_addr =  (struct sockaddr*)malloc(server_len);
	
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

	for(int t = 10;t>=0;t--)
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
#ifdef DEBUG
		printf("send %d bytes..\n", sizeof(struct calcProtocol));
#endif		

		rt = recvfrom(fd, (struct calcProtocol*) response, sizeof(struct calcProtocol), 0, reply_addr, &len);
		// resending
		if(rt ==-1)
		{
			puts("recv timeout, resending...");
			//fatal("Unable to receive msg, Exit.");
			continue;
		}
		if(rt != sizeof(struct calcProtocol))
		{
			fprintf(stderr, "%d %d version not applicable\n", rt, sizeof(struct calcProtocol));
			exit(1);	
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
	printf("arith: %d\n", response->arith);
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
// --- -
	// sleep(10);
	
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
		// recv result message

		int n = recvfrom(fd, (struct calcMessage *) msg, sizeof(struct calcMessage), 0, reply_addr, &len); 	
		if(rt == -1)
		{
			puts("recv timeout...");
			return 1;
		}

		parse_msg(msg);
#ifdef DEBUG
		printf("received %d,%d bytes from server\n",msg->message, n);
#endif	
		
			



	char* msg_sign[3]={
		"NOT APPLICABLE",
		"OK",
		"NOT OK"
	};
	printf("%s\n", msg_sign[msg->message]);
		
	free(response);
	free(msg);
	
}
