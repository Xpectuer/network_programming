/* You will to add includes here */
#include "common.h"
// Included to get the support library

// Enable if you want debugging to be printed, see examble below.
// Alternative, pass argument during compilation '-DDEBUG'
#define DEBUG


//using namespace std;


void init()
{
    setbuf(stdout,0); 
    setbuf(stdin,0);
    setbuf(stderr,0);
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
    
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int conn_fd;
	
	char GET_CONN_FREE[] = "GET_CONN_FREE";
	sem_t *mutex=sem_open(GET_CONN_FREE, O_CREAT , 0777, 1);

    while(1)
    {
		  			
          // Accept Connection 
          if((conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len)) < 0)
          {
              puts("accept failed!");
              exit(1);
          }
#ifdef DEBUG
		  printf("fd: %d\n", conn_fd);
#endif
		  if(conn_fd > MAX_FD_Q)
		  {
			  puts("too many connections");
		  	  close(conn_fd);
		  }
          char* cli_addr = inet_ntoa(client_addr.sin_addr);
          unsigned short pport = client_addr.sin_port;
          #ifdef DEBUG
          printf("[INFO] connection established from %s %hu\n", cli_addr, pport);
          #endif
		 // set timeout
		struct timeval read_timeout;
		read_timeout.tv_sec = 5;
		read_timeout.tv_usec = 0;
		setsockopt(conn_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&read_timeout, sizeof read_timeout);
	
          // -----handler----
          if(fork()==0)
		  {
			close(listen_fd);

			sem_wait(mutex);
			handler(conn_fd);
			sem_post(mutex);	
          	printf("Client %s:%hu hanged up\n",cli_addr, pport);
			exit(0);
		  } else {
			close(conn_fd);
			
		  }
          // ----------
          #ifdef DEBUG   
          #endif
    }
 
     
   // handler() 

    return 0;
}
