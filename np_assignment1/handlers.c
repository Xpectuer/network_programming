#include "common.h"



void on_client_timeout(int fd)
{
	char* buf ="ERROR TO";
	send(fd, buf, strlen(buf),0);
	close(fd);
	return;
}



// *enhencement of read call
// auto set 0 for buffer
// check for error
// auto set the last byte to 0.
// [5.1]set timeout handler
// set non-blocking enhencement
int read_plus(int fd, void* buf, size_t size)
{
	bzero(buf,size);
	
	int n = recv(fd, buf, size,0);
	if(n < 0)
	{
		
	    if(errno == EAGAIN || errno == EWOULDBLOCK)
		{
			puts("read timeout\n");
			on_client_timeout(fd);
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
	

	
	((char*)buf)[n] = 0;
	return n;
}

static void buf_fill(char* buf,char** protocols, int len)
{
	 bzero(buf,MAXLINE);
     int count = 0;
     int slen;
     for(int i=0; i < len; i++)
     {
        slen = strlen(protocols[i]);
        for(int j=0; j < slen;j++)
        {
             buf[count++] = protocols[i][j];
        } 
     }
} 



void handle_float(char *ptr,double *f1,double *f2,double *fresult)
{
	printf("Float\t");
	(*f1)=randomFloat();
	(*f2)=randomFloat();
	double ff1 = (*f1);
	double ff2 = (*f2);	
	/* At this point, ptr holds operator, f1 and f2 the operands. Now we work to determine the reference result. */
		
	if(strcmp(ptr,"fadd")==0){
	  *fresult=ff1+ff2;
	} else if (strcmp(ptr, "fsub")==0){
	  *fresult=ff1-ff2;
	} else if (strcmp(ptr, "fmul")==0){
	  *fresult=ff1*ff2;
	} else if (strcmp(ptr, "fdiv")==0){
	  *fresult=ff1/ff2;
	} else 
	{
		fatal("random lib error, unexcepted operator");
	}
} 

		
void handle_int(char *ptr, int *i1, int *i2, int *iresult)
{
	printf("Int\t");
 	(*i1)=randomInt();
	(*i2)=randomInt();
	int ii1 = (*i1);
	int ii2 = (*i2);
	if(strcmp(ptr,"add")==0){
	  *iresult=ii1+ii2;
	} else if (strcmp(ptr, "sub")==0){
	  *iresult=ii1-ii2;
	} else if (strcmp(ptr, "mul")==0){
	  *iresult=ii1*ii2;
	} else if (strcmp(ptr, "div")==0){
	  *iresult=ii1/ii2;
	} else {
		fatal("random lib error, unexcepted operator");
	}
}

void handler(int conn_fd)
{   
    //setbuf(conn_fd,0);
	
  	initCalcLib();

    char buf[MAXLINE];
    int len = 2;
    char* protocols[]={
        "TEXT TCP 1.0\n",
        "TEXT TCP 1.1\n\n",
    };
   	 
    buf_fill(buf, protocols, len);
	// load cmds then fill buf	
	#ifdef DEBUG
	printf("%s\n",buf);
	#endif
    if(send(conn_fd, buf, sizeof(buf), 0) < 0)
    {
        fatal("send error");
    }
     
    // wait for OK
    // ...........
	int n = read_plus(conn_fd, buf, MAXLINE);  
	if(strcmp(buf,"OK")!=0)
	{
		//puts("protocol not supported...");
		close(conn_fd);
		return;
	}
    #ifdef DEBUG
    printf("received %d bytes, which is %s\n", n, buf);
    #endif
	double f1,f2,fresult;
	int i1,i2,iresult;
	// operator oprand1 oprand2
	char* ptr;
	// operator
	ptr=randomType();	
	// oprands	
	bzero(buf,MAXLINE);
	int is_float=(ptr[0]=='f');	
	if(is_float)
	{
		handle_float(ptr,&f1,&f2,&fresult);
		sprintf(buf,"%s %8.8g %8.8g ?\n",ptr,f1,f2);
		printf("%s %8.8g %8.8g = %8.8g\n",ptr,f1,f2,fresult);	
	}
	else		 
	{
	    handle_int(ptr,&i1,&i2,&iresult); 	
		sprintf(buf,"%s %d %d ?\n",ptr,i1,i2);
	    printf("%s %d %d = %d \n",ptr,i1,i2,iresult);
	}
	if(send(conn_fd, buf, sizeof(buf),0) < 0)
	{
		fatal("send() error");
	} 
	// judgement
	n = read_plus(conn_fd,buf,sizeof(buf));
	//bzero(buf,sizeof(buf));
	char* res;
	if(is_float)
	{
		double ansd= strtod(buf, NULL);	
		bzero(buf,sizeof(buf));
		if(fabs(fresult - ansd) < 0.0001)
			res="OK\n";
		else
			res="ERROR\n";	
	}
	else
	{
		int ans = atoll(buf);
		bzero(buf,sizeof(buf));
		if(ans==iresult)
			res = "OK\n";
		else
			res = "ERROR\n";
	}	
	if(send(conn_fd, res, strlen(res),0) < 0)
	{
		fatal("send() error");
	}
	
	close(conn_fd);	
		
}
