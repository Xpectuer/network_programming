#include "tcp_server.h"

static void sig_int(int signo)
{
    info("gracefully shutdown");
    exit(0);
}

// returns connection file descriptor
int tcp_server(int port)
{
    int listen_fd;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int on = 1;
    // set reuse, to perform multiple client.
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int rt1 = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (rt1 < 0)
    {
        error(1, errno, "bind failed \n");
    }

    int rt2 = listen(listen_fd, LISTENQ);
    if (rt2 < 0)
    {
        error(1, errno, "listen failedi\n");
    }

    // ignore SIGPIPE to perform gracfully shutdown
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sig_int);

    int conn_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ((conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len)) < 0)
        error(1, errno, "bind failed\n");

    return conn_fd;
}

// return listen file descriptor
int tcp_listen(int port)
{

    int listen_fd;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int on = 1;
    // set reuse, to perform multiple client.
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int rt1 = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (rt1 < 0)
        error(1, errno, "bind failed");

    int rt2 = listen(listen_fd, LISTENQ);
    if (rt2 < 0)
        error(1, errno, "listen failed");

    // ignore SIGPIPE to perform gracfully shutdown
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sig_int);

    return listen_fd;
}
