#include "common.h"

// for testing
static void print_array(uint8_t *state, int n)
{
	fputs("[ ",stdout);
	for(int i=4;i<n;i++)
	{
		printf("%d ",state[i]);
	}
	puts("]");
}


int handler(int fd, context_t *server)
{

	
	int ret;
	uint8_t* state = server->state;
 	struct sockaddr_in  client_addr = server->addrs[fd];
	// int fd
	struct calcMessage _message;
	struct calcProtocol _protocol;
	

	_message.type=2;
	_message.message=0;
	_message.protocol=17;
	_message.major_version=1;
   	_message.minor_version=0;	

	
	uint8_t	s = state[fd];

	int iresult;
	double fresult;
	

#ifdef DEBUG
	print_array(server->state,10);
#endif
if(server->timeout[fd] == 1)
{
	goto end;	
}
	switch(s)
	{	
		case MSG_RCV:		
			// gen puzzle
			ret = gen_puzzle(&_protocol,&iresult,&fresult);
			if(ret == 1)
			{
				// returned float
				server->results[fd].fresult = fresult;			
			} 
			else if(ret == 0)
			{	
				server->results[fd].iresult = iresult;			
			}
#ifdef INFO
			printf("result: %d , %8.8g\n", iresult,fresult);	
#endif
			// send puzzle
			if(send_protocol(&_protocol, fd, &client_addr) < 0)
			{
				// not ready
				goto out;	
			}
			state[fd] = PRO_SET;
			break;		
		case PRO_SET:
			if(recv_protocol(&_protocol, fd,  &client_addr) < 0)
			{
				goto out;
			}
#ifdef INFO
			
			printf("<= received: %d , %8.8g\n", _protocol.inResult,_protocol.flResult);	
#endif
			// restore data from cache
			iresult = server->results[fd].iresult;
			fresult = server->results[fd].fresult;	

#ifdef INFO	
			printf("[|||] cache: %d , %8.8g\n", iresult,fresult);	
#endif
			// integer
			if(_protocol.arith < 5)
			{
				if(_protocol.inResult == iresult)
				{
					puts("OK");
					_message.message=1;
				}	
				else
				{
					_message.message=2;
				}
			}
			// float opts
			else if(_protocol.arith >= 5 && _protocol.arith <= 8)
			{
				if(fabs(_protocol.flResult - fresult) < 0.0001)
				{

					puts("OK");
					_message.message=1;					
				} 
				else
				{
					_message.message=2;
				}
			}	
			else 
			{	
					
				puts("Invalid Arith...Exit.");	
				
			}
			// cached
			server->mail_box[fd] = _message.message;	
					
			state[fd] = PRO_RCV;	
			break;
		case PRO_RCV:


			printf("[SERVER] client_id %d \n", fd-4);
			_message.message = server->mail_box[fd];
			if(send_msg(&_message, fd,  &client_addr)<0)
			{
				goto out;
			}
			state[fd] = MSG_END;
			break;
		case MSG_END:
			goto end;
			break;	
		case TIMEOUT:
			//puts("timeout");
			break;	
		}	
out:
	return 0;

end:
	return -1;
}


