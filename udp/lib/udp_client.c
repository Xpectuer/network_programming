#include "common.h"

int udp_client(char *ip_addr, int port)
{
    // 初始化socket
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip_addr, &server_addr.sin_addr);

    socklen_t server_len = sizeof(server_addr);

    // 响应报文
    struct sockaddr *reply_addr;
    reply_addr = malloc(server_len);

	// char send_line[MAXLINE], recv_line[MAXLINE + 1];
    socklen_t buf_len;
    int n;

    // define buffers and state enumarations
    char recv_buf[MAXLINE], send_buf[MAXLINE];

    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(socket_fd, &allreads);

    for (;;)
    {
        readmask = allreads;
        int rs = select(socket_fd + 1, &readmask, NULL, NULL, NULL);
        if (rs < 0)
            error(1, errno, "select failed");

        if (FD_ISSET(socket_fd, &readmask))
        {
            buf_len = 0;
            n = recvfrom(socket_fd, recv_buf, MAXLINE, 0, reply_addr, &buf_len);
            if (n < 0)
            {
                error(1, errno, "read failed");
            }
            recv_buf[n] = 0;
            fputs(recv_buf, stdout);
            fputs("\n", stdout);
        }
        if (FD_ISSET(STDIN_FILENO, &readmask))
        {
            if (fgets(send_buf, MAXLINE, stdin) != NULL)
            {
                int i = strlen(send_buf);
                if (send_buf[i - 1] == '\n')
                    send_buf[i - 1] = 0;
            }

            size_t rt = sendto(socket_fd,
                               send_buf, strlen(send_buf),
                               0,
                               (struct sockaddr *)&server_addr,
                               server_len);
            if (rt < 0)
            {
                error(1, errno, "send failed.");
            }
        }
    }

    return EXIT_SUCCESS;
}
