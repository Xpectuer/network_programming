#include "common.h"


int tcp_client(char* ip, int port) {
     // 0: deprecated argument for placeholder
     int socket_fd;
     struct sockeddr_in svr_addr;  
     
     if(ip==NULL)
        error(0,1,"Invalid Ip address!"); 
        
    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    
    bzero(&svr_addr, sizeof(svr_addr)); 
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htonl(port);
    
    inet_pton(AF_INET, ip, &svr_addr.sin_addr);
    // 3-way handshake
    int connect_rt = connect(socket_fd, (struct sockaddr *) &svr_addr, sizeof(svr_addr));
    
    if(connect_rt < 0) {
       error(1,0,"connection failed!"); 
    }
    
    return socket_fd;
}
