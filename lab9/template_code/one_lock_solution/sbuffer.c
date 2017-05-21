#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "sbuffer.h"


typedef struct sbuffer_node {
  struct sbuffer_node * next;
  sensor_data_t data;
} sbuffer_node_t;

struct sbuffer {
  sbuffer_node_t * head;
  sbuffer_node_t * tail;
	pthread_mutex_t buffer_mutex;
	pthread_cond_t readable_cond;
	//int readable_cond_flag;
};	


int sbuffer_init(sbuffer_t ** buffer)
{
  *buffer = malloc(sizeof(sbuffer_t));
  if (*buffer == NULL) return SBUFFER_FAILURE;
  (*buffer)->head = NULL;
  (*buffer)->tail = NULL;
	pthread_mutex_init(&((*buffer)->buffer_mutex) , NULL);
	pthread_cond_init(&((*buffer)->readable_cond), NULL);
	//(*buffer)->readable_cond_flag = 0;
  return SBUFFER_SUCCESS; 
}


int sbuffer_free(sbuffer_t ** buffer)
{
  sbuffer_node_t * dummy;
  if ((buffer==NULL) || (*buffer==NULL)) 
  {
    return SBUFFER_FAILURE;
  } 
  while ( (*buffer)->head )
  {
    dummy = (*buffer)->head;
    (*buffer)->head = (*buffer)->head->next;
    free(dummy);
  }
	pthread_mutex_destroy(&((*buffer)->buffer_mutex));
	pthread_cond_destroy(&((*buffer)->readable_cond));
  free(*buffer);
  *buffer = NULL;
  return SBUFFER_SUCCESS;		
}


int sbuffer_remove(sbuffer_t * buffer,sensor_data_t * data, int timeout)
{
  sbuffer_node_t * dummy;
	int retval;
	struct timespec abstime;

  if (buffer == NULL) return SBUFFER_FAILURE;
	//printf("buffer->readable_cond_flag = %d\n", buffer->readable_cond_flag);
	pthread_mutex_lock(&(buffer->buffer_mutex));
	//while(buffer->readable_cond_flag != 1)
	while(buffer->head == NULL)
	{
		abstime.tv_sec = time(NULL) + (time_t)timeout;
		abstime.tv_nsec = 0;
		retval = pthread_cond_timedwait(&(buffer->readable_cond), &(buffer->buffer_mutex), &abstime);
		if(retval == ETIMEDOUT)
		{
			pthread_mutex_unlock(&(buffer->buffer_mutex));
			return 1;
		}
	}

  *data = buffer->head->data;
  dummy = buffer->head;
  if (buffer->head == buffer->tail) // buffer has only one node
  {
    buffer->head = buffer->tail = NULL; 
		
  }
  else  // buffer has many nodes empty
  {
    buffer->head = buffer->head->next;
  }
/*
  if (buffer->head == NULL) 
	{
		buffer->readable_cond_flag = 0;
	}
*/
	pthread_mutex_unlock(&(buffer->buffer_mutex));
  free(dummy);
	
  return SBUFFER_SUCCESS;
}

int sbuffer_insert(sbuffer_t * buffer, sensor_data_t * data)
{
  sbuffer_node_t * dummy;
	pthread_mutex_lock(&(buffer->buffer_mutex));
  if (buffer == NULL) return SBUFFER_FAILURE;
  dummy = malloc(sizeof(sbuffer_node_t));
  if (dummy == NULL) return SBUFFER_FAILURE;
  dummy->data = *data;
  dummy->next = NULL;
  if (buffer->tail == NULL) // buffer empty (buffer->head should also be NULL
  {
    buffer->head = buffer->tail = dummy;
  } 
  else // buffer not empty
  {
    buffer->tail->next = dummy;
    buffer->tail = buffer->tail->next; 
  }
	//buffer->readable_cond_flag = 1;
	pthread_cond_broadcast(&(buffer->readable_cond));
//	puts("broadcast\n");
	pthread_mutex_unlock(&(buffer->buffer_mutex));
  return SBUFFER_SUCCESS;
}



