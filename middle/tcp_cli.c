#include "lib/common.h"

#define MAXLINE 1024

static void init()
{
    setbuf(stdout, 0);
    setbuf(stderr, 0);
    setbuf(stdin, 0);
}

int main(int argc, char **argv)
{
    init();
    if (argc != 3)
    {
        error(1, 0, "usage: tcp_cli <ipaddress> <port>\n");
    }
    int port = atoi(argv[2]);
    printf("connecting to ip:%s port:%d  ...\n", port, port);
    int socket_fd = tcp_client(argv[1], port);

    fprintf(stdout, "connection successful\n");

    //fputs(">\n", stdout);
    fputs("> ", stdout);
    //fflush(stdout);
    
    //getchar();

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
            error(1, errno, "select failed!\n");

        // read event: print msg to stdout
        if (FD_ISSET(socket_fd, &readmask))
        {
            // syscall read
            rs = read(socket_fd, recv_buf, MAXLINE);
            if (rs < 0)
            { // error occured
                error(1, errno, "read error\n");
            }
            else if (rs == 0)
            { // closed
                printf("server closed\n");
                break;
            }
            //printf("%s\n", recv_buf);
            cmd_info("response from server: %s", recv_buf);
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
                // exit cmd and gracefully shutdown
                if (strcmp(send_buf, "quit") == 0)
                {
                    if (shutdown(socket_fd, 1))
                    {
                        error(1, errno, "shutdown failed");
                    }
                }
                // do write
                size_t rt = write(socket_fd, send_buf, strlen(send_buf));
                if (rt < 0)
                {
                    error(1, errno, "write failed!");
                }
            }
        }
    }

    return 0;
}
