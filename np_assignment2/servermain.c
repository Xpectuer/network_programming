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

//using namespace std;
/* Needs to be global, to be rechable by callback and main */
int loopCount = 0;
int terminate = 0;

/* Call back function, will be called when the SIGALRM is raised when the timer expires. */
void checkJobbList(int signum)
{
  // As anybody can call the handler, its good coding to check the signal number that called it.

  printf("Let me be, I want to sleep.\n");

  if (loopCount > 20)
  {
    printf("I had enough.\n");
    terminate = 1;
  }

  return;
}

static void parse_msg(char* r, struct calcMessage *req, size_t n)
{  
	char **s = &r;
	req->type = ntohs(read_uint16(s));
	req->message = ntohl(read_uint32(s));
	req->protocol = ntohs(read_uint16(s));
	req->major_version = ntohs(read_uint16(s));	
	req->minor_version = ntohs(read_uint16(s));
}

void handler(int fd)
{
	struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
	char recv_buf[MAXLINE];
	int n = recvfrom(fd, recv_buf, MAXLINE, 0,
                         (struct sockaddr *)&client_addr, &client_len);
	if(n < 0)
	{
		fatal("Unable to recvfrom client, Exit.");
	}	
	struct calcMessage* req;
	req = malloc(sizeof(struct calcMessage));
	parse_msg(recv_buf, req, sizeof(req));
	#ifdef DEBUG
	print_bytes(sizeof(recv_buf),recv_buf);
	#endif
	printf("received %d bytes, and buf is:[ %s ]\n",n,recv_buf);
	printf("%d %d \n", req->type, req->major_version);
	free(req);
//        message[n] = 0;
//        
//        printf("received %d bytes: %s\n", n, message);
// 
//		sendto(socket_fd, (char*) &calc_msg,
//               sizof(calc_msg), 0, (struct sockaddr *)&client_addr, client_len);  
//
}

int main(int argc, char *argv[])
{

  /* Do more magic */

  /* 
     Prepare to setup a reoccurring event every 10s. If it_interval, or it_value is omitted, it will be a single alarm 10s after it has been set. 
  */
  struct itimerval alarmTime;
  alarmTime.it_interval.tv_sec = 10;
  alarmTime.it_interval.tv_usec = 10;
  alarmTime.it_value.tv_sec = 10;
  alarmTime.it_value.tv_usec = 10;

  char *arg = argv[1];

  char sign[] = ":";
  char *address = strtok(arg, sign);
  int port = atoi(strtok(NULL, sign));

  int listen_fd = udp_server(address, port);

  /* Regiter a callback function, associated with the SIGALRM signal, which will be raised when the alarm goes of */
  signal(SIGALRM, checkJobbList);
  setitimer(ITIMER_REAL, &alarmTime, NULL); // Start/register the alarm.


  printf("%s:%d\n", address, port);
  while (terminate == 0)
  {
    printf("This is the main loop, %d time.\n", loopCount);
    handler(listen_fd);
    loopCount++;
	
  }

  printf("done.\n");
  return (0);
}
