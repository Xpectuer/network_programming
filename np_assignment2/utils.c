#include "common.h"

double ntohlf(double x)
{
	int n = 1;
	// little endian if true
	char* f = (char*) &x;
	int tmp;
	if(*(char *)&n == 1) 
	{	
		for(int i = 0; i < 4;i++)
		{
			f[i] = tmp;f[i]=f[7-i];f[i]=f[7-i];
		}	
	}
	return x;
}


void parse_protocol(struct calcProtocol *response)
{
	response->type=ntohs(response->type);
	response->major_version=ntohs(response->major_version);		
	response->minor_version=ntohs(response->minor_version);
	response->id=ntohl(response->id);
	response->arith=ntohl(response->arith);
	response->inValue1=(uint32_t)ntohl(response->inValue1);
	response->inValue2=(uint32_t)ntohl(response->inValue2);
	response->inResult=(uint32_t)ntohl(response->inResult);
	response->flValue1=ntohlf(response->flValue1);
	response->flValue2=ntohlf(response->flValue2);
	response->flResult=ntohlf(response->flResult);
}

void parse_msg(struct calcMessage *msg)
{
	msg->type=ntohs(msg->type);
	msg->message=ntohl(msg->message);
	msg->protocol=ntohs(msg->protocol);
	msg->protocol=ntohs(msg->major_version);
	msg->protocol=ntohs(msg->minor_version);

}
