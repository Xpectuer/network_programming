#include "common.h"

#define LISTENQ 1024
/*
 *  tcp_server
 *  easy to use, quickly start a new point to point tcp server
 *  parameters: port : int
 *  returns: connection file descriptor
 * */
int tcp_server(int port);

/*
 *  tcp_listen
 *  for more flexible use. e.g. multiple client with one server
 *  parameters: port : int
 *  returns: listen file descriptor
 * */
int tcp_listen(int port);
