#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* You will to add includes here */
#include "common.h"
// Enable if you want debugging to be printed, see examble below.
// Alternative, pass
#define DEBUG

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

    int socket_fd = tcp_client(address, port);

    // define buffers and state enumarations
    char recv_buf[MAXLINE], send_buf[MAXLINE];
    int rs;

    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(socket_fd, &allreads);

    while (1)
    {

        readmask = allreads;
        int rc = select(socket_fd + 1, &readmask, NULL, NULL, NULL);
        if (rc <= 0)
            fatal("select error");

        if (FD_ISSET(socket_fd, &readmask))
        {
            bzero(recv_buf, sizeof(recv_buf));
            rs = read(socket_fd, recv_buf, MAXLINE);
            if (rs < 0)
            {
                fatal("read error");
            }
            else if (rs == 0)
            { // closed
                puts("service ended");
                break;
            }
            fprintf(stdout, "%s\n", recv_buf);
        }
        if (FD_ISSET(STDIN_FILENO, &readmask))
        {

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
        }
    }
}
