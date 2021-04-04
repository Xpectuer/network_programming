#include "common.h"

int tcp_client(char* ip_addr, int port) {
     // 0: deprecated argument for placeholder
     int socket_fd;
     struct sockaddr_in svr_addr;  
     
     if(ip_addr==NULL)
        error(0,1,"Invalid Ip address!"); 
        
    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    
    bzero(&svr_addr, sizeof(svr_addr)); 
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(port);
    
    inet_pton(AF_INET, ip_addr, &svr_addr.sin_addr);

    socklen_t server_len = sizeof(svr_addr); 
    // 3-way handshake
    int connect_rt = connect(socket_fd, (struct sockaddr *) &svr_addr, server_len);
    
    if(connect_rt < 0) {
       error(1,errno,"connection failed!\n"); 
    }
    
    return socket_fd;
}
