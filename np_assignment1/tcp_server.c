#include "common.h"

static void sig_int(int signo)
{
    puts("gracefully quit");
    exit(0);
}

// return listen file descriptor
int tcp_listen(char* address, int port)
{

    
    int listen_fd;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
 
    if(strcmp(address,"0.0.0.0") == 0)
    {
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        //puts("done!");
    }
    else
    { 
        inet_pton(AF_INET, address, &server_addr.sin_addr);
    }

    server_addr.sin_port = htons(port);

    int on = 1;
    // set reuse, to perform multiple client.
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	// --- bind ---

    int rt1 = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (rt1 < 0)
    {
        perror("bind failed");
        exit(1);
    }
    //    error(1, errno, "bind failed");
	

	
    int rt2 = listen(listen_fd, LISTENQ);
    if (rt2 < 0)
    {
        puts("listen failed");
        exit(1);
    }
    //  error(1, errno, "listen failed");

    // ignore SIGPIPE to perform gracfully shutdown
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sig_int);

    return listen_fd;
}
