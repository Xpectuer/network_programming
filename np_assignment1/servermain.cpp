//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
/* You will to add includes here */
#include "common.h"

// Included to get the support library
#include <calcLib.h>

// Enable if you want debugging to be printed, see examble below.
// Alternative, pass argument during compilation '-DDEBUG'
#define DEBUG


using namespace std;


void init()
{
    setbuf(stdout,0); 
    setbuf(stdin,0);
    setbuf(stderr,0);
}

static void buf_fill(char* buf,char** protocols, int len)
{
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

void handler(int conn_fd)
{   
    //setbuf(conn_fd,0);
    char buf[MAXLINE];
    bzero(buf, sizeof(buf));
    int len = 3;
    char* protocols[]={
        "TEXT TCP 1.0\n",
        "TEXT TCP 1.1\n",
        "\n"
    };
     
    while(1)
    {
        
        buf_fill(buf,protocols, len);
        #ifdef DEBUG
        printf("%s\n",buf);
        #endif
        if(send(conn_fd, buf, sizeof(buf), 0) < 0)
        {
            fatal("send error");
        }
                                    
        bzero(buf, sizeof(buf));
        // wait for OK
        // OK\n
        // <op> arg1 arg2 \n
        int n = read(conn_fd, buf, sizeof(buf));
        if(n < 0)
        {
            fatal("read error!");
        }
        if(n==0)
        {
            break;
        }
         
        // buf[n] = 0;
        
        #ifdef DEBUG
        fprintf(stdout,"buf is:%s\n",buf);
        #endif   
        if(strncmp(buf,"OK",2)==0)
        {
            #ifdef DEBUG
            printf("%s\n",buf); 
            #endif
            // skip  OK\n
            //char* buf_ptr = buf;
            //buf_ptr+=3;
           
            // do arithmetic 
            bzero(buf,sizeof(buf));
            n = read(conn_fd, buf, sizeof(buf));
            if(n<0)
            {
                fatal("read error!");
            }
            if(n==0)
            {
                return;
            }
            if(strncmp(buf,"OK",2)==0)
            {
                return;
            }
            char sign[]=" ";
            // puts("debug");
            char *opt = strtok(buf, sign);
            // puts("debug");
            double f1,f2,fresult;
            int i1,i2,iresult;

            // puts("debug1");
            char* res;
            // char* res = (char*) malloc(MAXLINE);    
            if(opt[0]=='f')
            { // float mode
                  #ifndef DEBUG 
                  printf("Float\t");
                  #endif  
                  char* f1_str = strtok(NULL, sign);
                  char* f2_str = strtok(NULL, sign);
                  
                  // Convert to float
                  f1 = strtod(f1_str, NULL); 
                  f2 = strtod(f2_str, NULL); 
                  /* At this point, ptr holds operator, f1 and f2 the operands. Now we work to determine the reference result. */
                 
                  if(strcmp(opt,"fadd")==0){
                    fresult=f1+f2;
                  } else if (strcmp(opt, "fsub")==0){
                    fresult=f1-f2;
                  } else if (strcmp(opt, "fmul")==0){
                    fresult=f1*f2;
                  } else if (strcmp(opt, "fdiv")==0){
                    fresult=f1/f2;
                  }
                  #ifdef DEBUG
                  printf("%s %8.8g %8.8g = %8.8g\n",opt,f1,f2,fresult);
                  #endif 
                  res = (char *)malloc(128);
                  sprintf(res,"%8.8g\n",fresult);
                  send(conn_fd, res, strlen(res), 0);
                  // free(res);
            } 
             else {
                  #ifndef DEBUG
                  printf("Int\t");
                  #endif
                  char* i1_str = strtok(NULL, sign);
                  char* i2_str = strtok(NULL, sign);
    
                  i1 = (int)strtol(i1_str, NULL,10);
                  i2 = (int)strtol(i2_str,NULL,10);
                  if(i1 == EINVAL|| i2 == EINVAL)
                  {
                      puts("Invalid Arguments Error !");
                      break;
                  }
    
  
                  
                  if(strcmp(opt,"add")==0){
                    iresult=i1+i2;
                  } else if (strcmp(opt, "sub")==0){
                    iresult=i1-i2;
                  } else if (strcmp(opt, "mul")==0){
                    iresult=i1*i2;
                  } else if (strcmp(opt, "div")==0){
                    iresult=i1/i2;
                  }
                  #ifdef DEBUG
                  printf("%s %d %d = %d \n",opt,i1,i2,iresult);
                  #endif 
                  res = (char*) malloc(128);  
                  sprintf(res,"%d\n", iresult);
                }
                
                send(conn_fd, res, strlen(res), 0);
                free(res);
            } 

        bzero(buf,sizeof(buf));
        n = read(conn_fd, buf, sizeof(buf));
        if(n<0)
        {
            fatal("read error!");
        }
        if(n==0)
        {
            return;
        }
        
        buf[n]=0;
        
        if(strncmp(buf,"OK",2) == 0)
        {
            return;
        }
                 
    }
       
     
}

int main(int argc, char *argv[]){
    init();
    if(argc != 2)
    {
        puts("usage: ./server <port>");
        return 1;
    }    
    /*
      Read first input, assumes <ip>:<port> syntax, convert into one string (Desthost) and one integer (port). 
       Atm, works only on dotted notation, i.e. IPv4 and DNS. IPv6 does not work if its using ':'. 
    */
    char delim[]=":";
    char *Desthost=strtok(argv[1],delim);
    char *Destport=strtok(NULL,delim);
    // *Desthost now points to a sting holding whatever came before the delimiter, ':'.
    // *Dstport points to whatever string came after the delimiter. 
    
    /* Do magic */
    char* address = Desthost;
    
    int port= (int)strtol(Destport, NULL,10);
    if(port == EINVAL)
    {
        puts("port string is not supported !");
        return 1;
    }
    
#ifdef DEBUG  
    printf("Host %s, and port %d.\n",Desthost,port);
#endif

    
    int listen_fd = tcp_listen(address, port);
    
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int conn_fd;
    while(1)
    {

          // Accept Connection
          if((conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len)) < 0)
          {
              puts("accept failed!");
              exit(1);
          }
          char* cli_addr = inet_ntoa(client_addr.sin_addr);
          unsigned short pport = client_addr.sin_port;
          #ifdef DEBUG
          printf("[INFO] connection established from %s %hu\n", cli_addr, pport);
          #endif
          // -----handler----
          handler(conn_fd);
          close(conn_fd);
          // ----------
          #ifdef DEBUG   
          printf("Client %s:%hu hanged up\n",cli_addr, pport);
          #endif
    }
 
     
   // handler() 

    return 0;
}
