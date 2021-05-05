#include "common.h"
#include "calcLib.h"
double ntohlf(double x)
{
	int n = 1;
	// little endian if true
	char* f = (char*) &x;
	int tmp;
	if(*(char *)&n == 1) 
	{	
		for(int i = 0; i < 4;i++)
		{
			tmp=f[i];f[i]=f[7-i];f[7-i]=tmp;
		}	
	}
	return x;
}

double htonlf(double x)
{
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


void parse_protocol_net(struct calcProtocol *response)
{
	response->type=htons(response->type);
	response->major_version=htons(response->major_version);		
	response->minor_version=htons(response->minor_version);
	response->id=htonl(response->id);
	response->arith=htonl(response->arith);
	response->inValue1=(uint32_t)htonl(response->inValue1);
	response->inValue2=(uint32_t)htonl(response->inValue2);
	response->inResult=(uint32_t)htonl(response->inResult);
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


void parse_msg_net(struct calcMessage *msg)
{
	msg->type=htons(msg->type);
	msg->message=htonl(msg->message);
	msg->protocol=htons(msg->protocol);
	msg->protocol=htons(msg->major_version);
	msg->protocol=htons(msg->minor_version);
}
int send_msg(struct calcMessage *msg, int fd, struct sockaddr_in *client_addr)
{
	parse_msg_net(msg);
	int ret;
	ret = sendto(fd, msg, sizeof(struct calcMessage), 0, (struct sockaddr *) client_addr, sizeof(struct sockaddr));
	if(ret > 0)
	{
#ifdef DEBUG
		printf("send %d bytes.\n", ret);
#endif
	} 
	else
	{
#ifdef DEBUG
		perror("send msg");
#endif

		if(errno == EWOULDBLOCK || errno == EAGAIN)
		{
			return -1;
		}
		exit(1);
	}
	return 0;
}

int recv_msg(struct calcMessage *msg, int fd, struct sockaddr_in *client_addr)
{
	parse_msg(msg);
	socklen_t len = sizeof(struct sockaddr);
	int ret;
	ret = recvfrom(fd, msg, sizeof(struct calcMessage), 0, (struct sockaddr *) client_addr, &len);
	if(ret > 0)
	{
#ifdef DEBUG
		printf("received %d bytes.\n", ret);
#endif
	} 
	else
	{
#ifdef DEBUG
		perror("recv msg");
#endif
		if(errno == EWOULDBLOCK || errno == EAGAIN)
		{

			return -1;
		}
		exit(1);
	}
	return 0; 

}

int send_protocol(struct calcProtocol *prtcl, int fd, struct sockaddr_in *client_addr)
{
	parse_protocol_net(prtcl);
	int ret;
	ret = sendto(fd, prtcl, sizeof(struct calcProtocol), 0, (struct sockaddr *) client_addr, sizeof(struct sockaddr_in));
	if(ret > 0)
	{
#ifdef DEBUG
		printf("send %d bytes.\n", ret);
#endif
	} 
	else
	{
#ifdef DEBUG
		perror("send protocol");
#endif	
		if(errno == EWOULDBLOCK || errno == EAGAIN)
		{
			return -1;
		}
		exit(1);
	}
	return 0;
}


int recv_protocol(struct calcProtocol *prtcl, int fd, struct sockaddr_in *client_addr)
{
	parse_protocol(prtcl);
	socklen_t len = sizeof(*client_addr);
	int ret;
	ret = recvfrom(fd, prtcl, sizeof(struct calcProtocol), 0, (struct sockaddr *) client_addr, &len);
	if(ret > 0)
	{
#ifdef DEBUG
		printf("received %d bytes.\n", ret);
#endif
	} 
	else
	{
#ifdef DEBUG
		perror("recv protocol");
#endif
		if(errno == EWOULDBLOCK || errno == EAGAIN)
		{
			return -1;
		}
		exit(1);
	}
	return 0;
}


void send_timeout(int fd, struct context_t *server)
{
	struct calcMessage msg ;
	msg.type=htons(3);
	msg.message=htons(0);
	msg.protocol=htons(17);
	msg.major_version=htons(1);
	msg.minor_version=htons(0);	
	parse_msg_net(&msg);
	send_msg(&msg, fd, &server->addrs[fd]);
}

#define ADD 1
#define SUB 2
#define MUL 3 
#define DIV 4
#define FADD 5
#define FSUB 6
#define FMUL 7
#define FDIV 8



// float 1
// int 0
//
int gen_puzzle(struct calcProtocol* protocol, int* iresult, double *fresult)
{
	initCalcLib();
	char* ptr = randomType();	
	int i1,i2;
	double f1, f2;
	// float
	if(ptr[0]=='f'){
	  printf("Float\t");
	  f1=randomFloat();
	  f2=randomFloat();
	  protocol->flValue1=f1;
  	  protocol->flValue2=f2;	  
	  /* At this point, ptr holds operator, f1 and f2 the operands. Now we work to determine the reference result. */
	  if(strcmp(ptr,"fadd")==0){
	    *fresult=f1+f2;
		protocol->arith = FADD;
	  } else if (strcmp(ptr, "fsub")==0){
	    *fresult=f1-f2;
		protocol->arith = FSUB;
	  } else if (strcmp(ptr, "fmul")==0){
	    *fresult=f1*f2;
		protocol->arith = FMUL;
	  } else if (strcmp(ptr, "fdiv")==0){
	    *fresult=f1/f2;
		protocol->arith = FDIV;
	  }
	  printf("%s %8.8g %8.8g = %8.8g\n",ptr,f1,f2,*fresult);
	  return 1;
	} 
	// integer
	printf("Int\t");
	i1=randomInt();
	i2=randomInt();
	protocol->inValue1=i1;
  	protocol->inValue2=i2;
	if(strcmp(ptr,"add")==0){
	  *iresult=i1+i2;
	  protocol->arith = ADD;
	} else if (strcmp(ptr, "sub")==0){
	  *iresult=i1-i2;
	  protocol->arith = SUB;
	} else if (strcmp(ptr, "mul")==0){
	  *iresult=i1*i2;
	  protocol->arith = MUL;
	} else if (strcmp(ptr, "div")==0){
	  *iresult=i1/i2;
	  protocol->arith = DIV;
	}
	
	printf("%s %d %d = %d \n",ptr,i1,i2,*iresult);
	return 0;
	
}
