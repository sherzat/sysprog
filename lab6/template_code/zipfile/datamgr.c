#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "datamgr.h"
#include "./lib/dplist.h"
#include "config.h"

void * element_copy(void *element)
{
	sensor_node_t * copy;
	copy = malloc(sizeof(sensor_node_t));
	if (copy == NULL)
	{
		fprintf(stderr, "\nIn %s - function %s at line %d: failed\n", __FILE__, __func__, __LINE__);
		exit(EXIT_FAILURE);
	}
	
	copy->sensor_id = ((sensor_node_t *)element)->sensor_id;
	copy->room_id = ((sensor_node_t *)element)->room_id;
	copy->last_modified = ((sensor_node_t *)element)->last_modified;
	copy->index = ((sensor_node_t *)element)->index;
	copy->buffer_size = ((sensor_node_t *)element)->buffer_size;
	memcpy(copy->buffer, ((sensor_node_t *)element)->buffer, sizeof(copy->buffer));

	return copy;
}

void element_free(void **element)
{
	free(*element);
	*element = NULL;
}

int element_compare(void *x, void *y)
{
	if (((sensor_node_t *)x)->sensor_id == ((sensor_node_t *)y)->sensor_id)
		return 0;
	else if (((sensor_node_t *)x)->sensor_id > ((sensor_node_t *)y)->sensor_id)
		return 1;
	return -1;
}

dplist_t *list =NULL;
int dplist_errno;
 
void datamgr_parse_sensor_files(FILE * fp_sensor_map, FILE * fp_sensor_data)
{

	sensor_data_t * sensor_data = NULL;
	sensor_data = malloc(sizeof(sensor_data_t));
	if (sensor_data == NULL)
	{
		fprintf(stderr, "failed to malloc\n");
		exit(EXIT_FAILURE);
	}

	sensor_node_t * sensor_node = NULL;
	sensor_node = malloc(sizeof(sensor_node_t));
	if (sensor_node == NULL)
	{
		fprintf(stderr, "\nIn %s - function %s at line %d: failed\n", __FILE__, __func__, __LINE__);
		exit(EXIT_FAILURE);
	}
	sensor_node->index = 0;
	sensor_node->buffer_size = 0;
//	sensor_node->room_id = 0;
//	sensor_node->sensor_id = 0;
//	sensor_node->last_modified = 0;
	memset(sensor_node->buffer, 0, sizeof(sensor_node->buffer));


	list = dpl_create(element_copy, element_free, element_compare);
	if (list == NULL)
	{
		fprintf(stderr, "\nIn %s - function %s at line %d: failed\n", __FILE__, __func__, __LINE__);
		exit(EXIT_FAILURE);
	}

	while(fscanf(fp_sensor_map,"%" SCNu16 " %" SCNu16 "\n", &sensor_node->room_id, &sensor_node->sensor_id) != EOF)
	{
		//printf("room_id %" PRIu16",  sensor_id %"PRIu16"\n", sensor_node->room_id, sensor_node->sensor_id);
		sensor_node_t *temp = datamgr_get_sensnor_node(sensor_node->room_id);
		if(temp != NULL)
		{
			fprintf(stderr, "sensor with sensor_id = %"PRIu16" is already mapped\n", sensor_node->sensor_id);
			continue;
		}

		dpl_insert_at_index(list, sensor_node, 0, true);
	}
	free(sensor_node);
	
/*
	j = dpl_size(list);
	for (i = 0; i<j; i++)
	{
		sensor_node_t * element = dpl_get_element_at_index(list, i);
		printf("element at index %d = %"PRIu16" %" PRIu16" %d   %d\n", i, element->room_id, element->sensor_id, element->index, element->buffer_size);
	}
*/
	while (1)
	{
		fread(&sensor_data->id, sizeof(sensor_id_t), 1, fp_sensor_data);
		fread(&sensor_data->value, sizeof(sensor_value_t), 1, fp_sensor_data);
		fread(&sensor_data->ts, sizeof(sensor_ts_t), 1, fp_sensor_data);
		if (feof(fp_sensor_data) != 0)
			break;
	//	printf("sensor_id %" PRIu16"\nsensor_value %lf\nsensor_ts %ld\n", sensor_data->id, sensor_data->value, sensor_data->ts);

		sensor_node_t *temp = datamgr_get_sensnor_node(sensor_data->id);
		if(temp == NULL)
		{
			//fprintf(stderr, "sensor with sensor_id = %"PRIu16" is not mapped\n", sensor_data->id);
			continue;
		}
		temp->buffer[temp->index++] = sensor_data->value; 
		temp->last_modified = sensor_data->ts;
		if(temp->buffer_size < RUN_AVG_LENGTH)
			temp->buffer_size++;

		if(temp->index >= RUN_AVG_LENGTH)
			temp->index = 0;

		sensor_value_t running_avg = 0;
		running_avg =  datamgr_running_avg(temp->sensor_id);
		if(running_avg == 0)
		{
			continue;
		}
		if(running_avg > SET_MAX_TEMP)
		{
			fprintf(stderr, "running_avg %f > max_temp %f , in room %"PRIu16" is too hot! \n", running_avg, (double)SET_MAX_TEMP, temp->room_id);

		}
		if(running_avg < SET_MIN_TEMP)
		{
			fprintf(stderr, "running_avg %f < min_temp %f, in room %"PRIu16" is too cold \n", running_avg, (double)SET_MIN_TEMP, temp->room_id);
		}

/******print sensor_node******
		printf("sensor_id = %"PRIu16"  room_id = %"PRIu16"  last_modified = %ld  index = %d  buffer_size = %d  ", 
				temp->sensor_id, temp->room_id, temp->last_modified, temp->index, temp->buffer_size);
		printf("buffer [");
		for(int i = 0; i < RUN_AVG_LENGTH; i++)
		{
			printf("%f, ",temp->buffer[i]);
		}
		puts("]");
*/		
	}
	free(sensor_data);
}

void datamgr_free()
{
	dpl_free(&list);
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id)
{
	sensor_node_t *sensor_node = NULL;
	sensor_node = datamgr_get_sensnor_node(sensor_id);
	if (sensor_node == NULL)
	{
		//fprintf(stderr, "sensor_id %"PRIu16" is not mapped with any room_id\n", sensor_id);
		return 0;
	}

	return sensor_node->room_id;
}

sensor_value_t datamgr_running_avg(sensor_id_t sensor_id)
{
	sensor_node_t *sensor_node = NULL;
	sensor_node = datamgr_get_sensnor_node(sensor_id);
	if (sensor_node == NULL)
	{
		//fprintf(stderr, "sensor_id %"PRIu16" is not mapped with any room_id\n", sensor_id);
		return -1;
	}

	sensor_value_t running_sum = 0;
	if (sensor_node->buffer_size < 5)
	{
		//fprintf(stderr, "sensor_id = %"PRIu16", less than RUN_AVG_LENGTH = %d measurements are recorded, the avg is 0\n",sensor_node->sensor_id, RUN_AVG_LENGTH);
		return 0;
	}
	for(int i =0; i < RUN_AVG_LENGTH; i++)
	{
		running_sum += sensor_node->buffer[i];
	}

	return running_sum/RUN_AVG_LENGTH;
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id)
{
	sensor_value_t ret_val = datamgr_running_avg(sensor_id);
	if (ret_val == 0)
	{
		//fprintf(stderr, "sensor_id = %"PRIu16", less than RUN_AVG_LENGTH = %d measurements are recorded, the avg is 0\n", sensor_id, RUN_AVG_LENGTH);
	}
	return ret_val;
}
time_t datamgr_get_last_modified(sensor_id_t sensor_id)
{
	sensor_node_t *sensor_node;
	sensor_node = datamgr_get_sensnor_node(sensor_id);
	if (sensor_node == NULL)
	{
		//fprintf(stderr, "sensor_id %"PRIu16" is not mapped with any room_id\n", sensor_id);
		return 0;
	}

	return sensor_node->last_modified;
}

int datamgr_get_total_sensors()
{
	int ret_val = 0; 
	ret_val = dpl_size(list);
	
	if(dplist_errno != DPLIST_NO_ERROR)
	{
		//fprintf(stderr, "\nIn %s - function %s at line %d: failed \n", __FILE__, __func__, __LINE__);
		exit(EXIT_FAILURE);
	}
	
	return ret_val;
}

sensor_node_t *datamgr_get_sensnor_node(sensor_id_t sensor_id)
{
	sensor_node_t *sensor_node=NULL;
	dplist_node_t *list_node=NULL;
	sensor_node = malloc(sizeof(sensor_node_t));
	if (sensor_node == NULL)
	{
		fprintf(stderr, "\nIn %s - function %s at line %d: failed\n", __FILE__, __func__, __LINE__);
		exit(EXIT_FAILURE);
	}
	sensor_node->sensor_id = sensor_id;
	if((list_node = dpl_get_reference_of_element(list, sensor_node)) == NULL)
	{
		if(dplist_errno != DPLIST_NO_ERROR)
		{
			fprintf(stderr, "\nIn %s - function %s at line %d: failed \n", __FILE__, __func__, __LINE__);
			exit(EXIT_FAILURE);
		}
		return NULL;
	}
	free(sensor_node);
	sensor_node = dpl_get_element_at_reference(list, list_node);
	return sensor_node;
}

