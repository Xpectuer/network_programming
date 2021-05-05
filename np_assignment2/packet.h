void on_client_timeout(int fd,struct sockaddr *client_addr, socklen_t addr_len);

#define NA 0
#define OK 1 
#define NOK 2
void send_msg(struct calcMessage *msg, int fd, uint32_t message, struct sockaddr_in *client_addr);

void recv_msg(struct calcMessage *msg, int fd, uint32_t *message, struct sockaddr_in *client_addr);


void send_protocol(struct calcProtocol *prtcl, int fd, struct sockaddr_in *client_addr);
		

void gen_puzzle(struct calcProtocol* protocol, int* iresult, double *fresult);
