#include "connmgr.h"
#include "./lib/tcpsock.h"
#include "./lib/dplist.h"
#include "config.h"

//#define MAX_CONN 3  // state the max. number of connections the server will handle before exiting
/*
#if defined(TIMEOUT)
#else
	#error "TIMEOUT not defined\n"
#endif
*/
#ifndef TIMEOUT
#error "TIMEOUT not defined\n"
#endif

#define FILE_ERROR(fp,error_msg) 	do { \
					  if ((fp)==NULL) { \
					    printf("%s\n",(error_msg)); \
					    exit(EXIT_FAILURE); \
					  }	\
					} while(0)

struct connection{
	tcpsock_t *client;
	time_t timestamp;
} connection;

dplist_t *conn_list = NULL;
tcpsock_t * server, * client;
fd_set readfds;
int dplist_errno;

void * element_copy(void *element)
{
	connection_t *copy;
	copy = malloc(sizeof(*copy));
	if (copy == NULL)
	{
		fprintf(stderr, "\nIn %s - function %s at line %d: failed\n", __FILE__, __func__, __LINE__);
		exit(EXIT_FAILURE);
	}	
	copy->client = ((connection_t *)element)->client;
	copy->timestamp =((connection_t *)element)->timestamp;

	return copy;
}

void element_free(void **element)
{
	free(*element);
	*element = NULL;
}

int element_compare(void *x, void *y)
{
	int client_sdx, client_sdy;
	tcp_get_sd(((connection_t *)x)->client, &client_sdx);
	tcp_get_sd(((connection_t *)y)->client, &client_sdy);

	if(client_sdx == client_sdy)
		return 0;
	if(client_sdx > client_sdy)
		return 1;
	return -1;
}

void handle_new_connection()
{			
	if (tcp_wait_for_connection(server,&client)!=TCP_NO_ERROR) exit(EXIT_FAILURE);
	connection_t * conn = NULL;

	conn = malloc(sizeof(*conn));
	if (conn == NULL)
	{
		fprintf(stderr, "failed to malloc\n");
		exit(EXIT_FAILURE);
	}
	conn->client = client;
	conn->timestamp = time(NULL);

	dpl_insert_at_index(conn_list, conn, dpl_size(conn_list), true); /* save client */
	printf("Incoming client connection\n");
	free(conn);
}

void write_data_to_file(FILE *fp_bin, sensor_id_t id, sensor_value_t value, sensor_ts_t ts)
{
  fwrite(&id, sizeof(id), 1, fp_bin);
	fwrite(&value, sizeof(value), 1, fp_bin);
 	fwrite(&ts, sizeof(ts), 1, fp_bin);
	if (ferror(fp_bin) != 0)
	{
		printf("failed to write_data_to_file\n");
		exit(EXIT_FAILURE);
	}	
}

void handle_new_data(FILE * fp_bin)
{
  sensor_data_t data;
  int bytes, result;
	int client_sd;

	dplist_node_t * ref = dpl_get_first_reference(conn_list);
	while(ref != NULL)/* check all the clients  for data */
	{
		connection_t *conn = NULL;
		conn = dpl_get_element_at_reference(conn_list, ref);
		tcp_get_sd(conn->client, &client_sd);
		if(FD_ISSET(client_sd, &readfds))
		{
			// read sensor ID
			bytes = sizeof(data.id);
			tcp_receive(conn->client,(void *)&data.id,&bytes);
			// read temperature
			bytes = sizeof(data.value);
			tcp_receive(conn->client,(void *)&data.value,&bytes);
			// read timestamp
			bytes = sizeof(data.ts);
			result = tcp_receive(conn->client, (void *)&data.ts,&bytes);
			if ((result==TCP_NO_ERROR) && bytes) 
			{
				conn->timestamp = time(NULL);
				printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value, (long int)data.ts);
				write_data_to_file(fp_bin, data.id, data.value, data.ts);
			}
			else
			{
				tcp_close(&conn->client);
				FD_CLR(client_sd, &readfds);
				dplist_node_t *tmp = ref;
				ref = dpl_get_previous_reference(conn_list, ref);
				dpl_remove_at_reference(conn_list, tmp, true);

				if (result==TCP_CONNECTION_CLOSED) 
					printf("Peer has closed connection\n");
				else
					printf("Error occured on connection to peer\n");
			}
		}
		ref = dpl_get_next_reference(conn_list, ref);	
	}
}

void build_select_readfds(int * max_fd)
{
	int client_sd;
	int listen_fd;
	tcp_get_sd(server, &listen_fd);
	FD_ZERO(&readfds);

	FD_SET(listen_fd, &readfds);

	dplist_node_t * ref = dpl_get_first_reference(conn_list);
	while(ref != NULL)/* check all the clients  for data */
	{
		long int t = 0;
		connection_t *conn = NULL;
		conn = dpl_get_element_at_reference(conn_list, ref);
		if ((t = time(NULL) - conn->timestamp) >= TIMEOUT)
		{
			tcp_close(&conn->client);
			printf("connection timout, disconnect peer %ld\n", t);
			dplist_node_t *tmp = ref;
			ref = dpl_get_next_reference(conn_list, ref);
			dpl_remove_at_reference(conn_list, tmp, true);
		}
		else
		{
			tcp_get_sd(conn->client, &client_sd);
			FD_SET(client_sd, &readfds); /* add new client descriptor into readfds */
			*max_fd = ((*max_fd) > client_sd ? *max_fd : client_sd);
			ref = dpl_get_next_reference(conn_list, ref);
		}
	}
	//printf("in build_select_readfds, max_fd=%d\n", max_fd);
}

void connmgr_listen(int port_number)
{
	struct timeval tv;
	int listen_fd;
	int max_fd;
	
  FILE * fp_bin;
  fp_bin = fopen("sensor_data_recv", "w");
  FILE_ERROR(fp_bin,"Couldn't create sensor_data\n");  

  printf("Test server is started\n");
  if (tcp_passive_open(&server,port_number)!=TCP_NO_ERROR) exit(EXIT_FAILURE);
		
	tcp_get_sd(server, &listen_fd);
	max_fd = listen_fd;

	conn_list = dpl_create(element_copy, element_free, element_compare);

	do
	{
		int nready;
		build_select_readfds(&max_fd);
	 	tv.tv_sec = TIMEOUT;
		tv.tv_usec = 0;
		nready = select(max_fd+1, &readfds, NULL, NULL, &tv);
		if (nready < 0)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}
		else if (nready == 0)
		{
			printf("connection time out\n");
			if (tcp_close( &server )!=TCP_NO_ERROR) exit(EXIT_FAILURE);
 		 	printf("Test server is shutting down\n");
			break;
		}
		else
		{
			if (FD_ISSET(listen_fd, &readfds)) /* new client connection */
				handle_new_connection();
			handle_new_data(fp_bin);
		}
	}while(1);
	fclose(fp_bin);

  //if (tcp_close( &server )!=TCP_NO_ERROR) exit(EXIT_FAILURE);
 // printf("Test server is shutting down\n");
	
}

void connmgr_free()
{
	dpl_free(&conn_list);
}
