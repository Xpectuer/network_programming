#include "lib/common.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        error(1, 0, "udpclient <ipAddress> <port>");
    }
    int port = atoi(argv[2]);
    char *ip_addr = argv[1];
    udp_client(ip_addr, port);
}
