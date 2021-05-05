#include "common.h"

void send_msg(struct calcMessage *msg, int fd, uint32_t message, struct sockaddr_in *client_addr)
{
	msg->message = message;
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
		perror("recv msg");
#endif

		if(errno == EWOULDBLOCK || errno == EAGAIN)
		{
			return;
		}
		exit(1);
	}
}

void recv_msg(struct calcMessage *msg, int fd, uint32_t *message, struct sockaddr_in *client_addr)
{
	socklen_t len = sizeof(*client_addr);
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

			return;
		}
		exit(1);
	}
	message = msg->message;
}

void send_protocol(struct calcProtocol *prtcl, int fd, struct sockaddr_in *client_addr)
{
	int ret;
	ret = sendto(fd, prtcl, sizeof(struct calcProtocol), 0, (struct sockaddr *) client_addr, sizeof(struct sockaddr));
	if(ret > 0)
	{
#ifdef DEBUG
		printf("send %d bytes.\n", ret);
#endif
	} 
	else
	{
#ifdef DEBUG
		perror("recv msg");
#endif	
		if(errno == EWOULDBLOCK || errno == EAGAIN)
		{
			return;
		}
		exit(1);
	}
}

#define ADD 1
#define SUB 2
#define MUL 3 
#define DIV 4
#define FADD 5
#define FSUB 6
#define FMUL 7
#define FDIV 8


int gen_puzzle(struct calcProtocol* protocol, int* iresult, double *fresult)
{
	char* ptr = randomType();	
	
	// float
	if(ptr[0]=='f'){
	  printf("Float\t");
	  f1=randomFloat();
	  f2=randomFloat();
	
	  /* At this point, ptr holds operator, f1 and f2 the operands. Now we work to determine the reference result. */
	  if(strcmp(ptr,"fadd")==0){
	    *fresult=f1+f2;
		protocol->arith = FADD;
	  } else if (strcmp(ptr, "fsub")==0){
	    *fresult=f1-f2;
		protocol->arith = FSUB;
	  } else if (strcmp(ptr, "fmul")==0){
	    *fresult=f1*f2;
		protocol->atith = FMUL;
	  } else if (strcmp(ptr, "fdiv")==0){
	    *fresult=f1/f2;
		protocol->arith = FDIV;
	  }
	  printf("%s %8.8g %8.8g = %8.8g\n",ptr,f1,f2,fresult);
	  return 1;
	} 
	// integer
	printf("Int\t");
	i1=randomInt();
	i2=randomInt();
	
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
	
	printf("%s %d %d = %d \n",ptr,i1,i2,iresult);
	return 0;
	



}
