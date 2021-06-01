#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* You will to add includes here */
#include "common.h"
// Enable if you want debugging to be printed, see examble below.
// Alternative, pass
//#define DEBUG

// Included to get the support library
#include <calcLib.h>

void init()
{
    setbuf(stdout, 0);
    setbuf(stdin, 0);
    setbuf(stderr, 0);
}

static char *parseURL(char *Desthost)
{
    //TODO: to add a DNS parse function
     
    return Desthost;
}

static char*  handle_cal(char *buf, int size)
{
	char delim[] = " ";
	char * str_array[3];
	//init array
	for(int i=0;i<3;i++)
	{
		str_array[i] = malloc(MAXLINE);
	}
	
	strcpy(str_array[0], strtok(buf, delim));
	strcpy(str_array[1], strtok(NULL, delim));
	strcpy(str_array[2], strtok(NULL,delim));
#ifdef DEBUG	
	for(int i = 0; i < 3;i++)
	{
		printf("%s\n", str_array[i]);
	}
	
#endif
	char *ptr = str_array[0];
	double f1,f2,fresult;
	int i1,i2,iresult;
	char *sresult = malloc(1024);
  	if(ptr[0]=='f')
	{
			f1= strtod(str_array[1],NULL);
			f2= strtod(str_array[2],NULL);
	
			if(strncmp(ptr,"fadd",4)==0){
			  fresult=f1+f2;
			} else if (strncmp(ptr, "fsub",4)==0){
			  fresult=f1-f2;
			} else if (strncmp(ptr, "fmul",4)==0){
			  fresult=f1*f2;
			} else if (strncmp(ptr, "fdiv",4)==0){
			  fresult=f1/f2;
			}
			sprintf(sresult, "%lf", fresult);
  	} 
	else 
	{
			i1 = atoi(str_array[1]);
			i2 = atoi(str_array[2]);

			if(strncmp(ptr,"add",3)==0){
			  iresult=i1+i2;
			} else if (strncmp(ptr, "sub",3)==0){
			  iresult=i1-i2;
			} else if (strncmp(ptr, "mul",3)==0){
			  iresult=i1*i2;
			} else if (strncmp(ptr, "div",3)==0){
			  iresult=i1/i2;
			}

			sprintf(sresult, "%d", iresult);

  	}

	for(int i=0;i<3;i++)
	{
		free(str_array[i]);
	}
	return sresult;
}

int main(int argc, char *argv[])
{
    init();

    //TODO: to add a DNS parse function

    /*
      Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
       Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
    */
    char delim[] = ":";
    char *Desthost = strtok(argv[1], delim);
    char *Destport = strtok(NULL, delim);
    // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
    // *Dstport points to whatever string came after the delimiter.
    char *address = parseURL(Desthost);
    /* Do magic */
    int port = atoi(Destport);

#ifdef DEBUG
    printf("Host %s, and port %d.\n", Desthost, port);
#endif

	char error[1024] = "ERROR: max queue size reached!";
    int socket_fd = tcp_client(address, port);

    // define buffers and state enumarations
    char recv_buf[MAXLINE], send_buf[MAXLINE];
    int rs;

	rs = read(socket_fd, recv_buf, MAXLINE);
	if(rs == 0)
	{
		close(socket_fd);
		exit(0);
	}
	if(rs < 0)
	{
		perror("read error!");
		exit(1);
	}
	fprintf(stdout,"%s\n", recv_buf);
	// send OK
	if (fgets(send_buf, MAXLINE, stdin) != NULL)
    {
	 		int i = strlen(send_buf);
	 		if (send_buf[i - 1] == '\n')
	 		{
	 			send_buf[i - 1] = '\0';
	 		}

	 		// do write
	 		size_t rt = write(socket_fd, send_buf, strlen(send_buf));
	 		if (rt < 0)
	 		{
	 			fatal("write failed!");
	 		}
     }
	 // receive puzzle 
     bzero(recv_buf, sizeof(recv_buf));
     rs = read(socket_fd, recv_buf, MAXLINE);
     if (rs < 0)
     {
         fatal("read error");
     }
     else if (rs == 0)
     { 
		 // closed
         puts("service ended");
		 close(socket_fd);
     }
	 else if (strcmp(recv_buf,"ERROR TO")==0)
	 {
		 close(socket_fd);
		 puts("timeout");
		 exit(1);
	 }
	 char *srs;
	 srs = handle_cal(recv_buf,sizeof(recv_buf));


	 printf("result: %s\n",srs);
	 size_t rt = write(socket_fd, srs, sizeof(send_buf));
	 if(rt < 0)
	 {
		perror("write");
		exit(1);
	 } 
	    
     rt = read(socket_fd, recv_buf, sizeof(recv_buf));            
     if(rt < 0)
	 {
		perror("read");
		exit(1);
	 }
	 	
	 fprintf(stdout, "%s\n",recv_buf);

	 return 0;
}
