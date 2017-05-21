#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <unistd.h>
#include "config.h"
#include "tcpsock.h"

#define PORT 1234
#define MAX_CONN 3  // state the max. number of connections the server will handle before exiting


/* Implements a parallal test server (more than one connection at the same time)
 */

int main(int argc,char *argv[])
{
  tcpsock_t * server, * client;
  sensor_data_t data;
  int bytes, result;
  int conn_counter = 0;
	pid_t pid;
	
  printf("Test server is started\n");
  if (tcp_passive_open(&server,PORT)!=TCP_NO_ERROR) exit(EXIT_FAILURE);

	do
	{
    if (tcp_wait_for_connection(server,&client)!=TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Incoming client connection\n");
    conn_counter++;

	//	printf("clientaddr-->IP:%s\n",inet_ntoa(clientaddr.sin_addr));
	//	printf("clientaddr-->PORT: %d \n",ntohs(clientaddr.sin_port));
	//
		if((pid = fork()) < 0)
		{
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			close(&server);
			do
			{
				// read sensor ID
				bytes = sizeof(data.id);
				result = tcp_receive(client,(void *)&data.id,&bytes);
				// read temperature
				bytes = sizeof(data.value);
				result = tcp_receive(client,(void *)&data.value,&bytes);
				// read timestamp
				bytes = sizeof(data.ts);
				result = tcp_receive( client, (void *)&data.ts,&bytes);
				if ((result==TCP_NO_ERROR) && bytes) 
				{
					printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value, (long int)data.ts);
				}
			} while (result == TCP_NO_ERROR);
  	  if (result==TCP_CONNECTION_CLOSED) 
				printf("Peer has closed connection\n");
    	else
      	printf("Error occured on connection to peer\n");
    	tcp_close(&client);
			
			exit(EXIT_SUCCESS);
		}
		else
		{
    	tcp_close(&client);
			waitpid(-1, NULL, WNOHANG);

		}
	}while(conn_counter < MAX_CONN);
  if (tcp_close( &server )!=TCP_NO_ERROR) exit(EXIT_FAILURE);
  printf("Test server is shutting down\n");
	
	return 0;
}
