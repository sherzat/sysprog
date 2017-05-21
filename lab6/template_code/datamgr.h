#ifndef _DATAMGR_H_
#define _DATAMGR_H_



#include "config.h"
#include <stdio.h>



void datamgr_parse_sensor_files(FILE * fp_sensor_map, FILE * fp_sensor_data);
//This method holds the core functionality of your datamgr. It takes in 2 file
//pointers to the sensor files and parses them. When the method finishes all
//data should be in the internal pointer list and all log messages should be
//printed to stderr

void datamgr_free();
//This method should be called to clean up the datamgr, and to free all used
//memory. After this, any call to datamgr_get_room_id, datamgr_get_avg,
//datamgr_get_last_modified or datamgr_get_total_sensors will not return a valid
//result

uint16_t datamgr_get_room_id(sensor_id_t sensor_id);
//Gets the room ID for a certain sensor ID
//if the datamgr is not created or already freed then exit(EXIT_FAILURE).
//if the sensor_id is not found return  0. 

sensor_value_t datamgr_running_avg(sensor_id_t sensor_id);
//calculates the running_avg of a certain sensor_id.

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id);
//Gets the running AVG of a certain senor ID (if less then RUN_AVG_LENGTH
//measurements are recorded the avg is 0)
//if the sensor_id is not found return  0. 

time_t datamgr_get_last_modified(sensor_id_t sensor_id);
//Returns the time of the last reading for a certain sensor ID

int datamgr_get_total_sensors();
//Return the total amount of unique sensor ID's recorded by the datamgr

sensor_node_t *datamgr_get_sensnor_node(sensor_id_t sensor_id);
//if the datamgr is not created or already freed then return NULL;

#endif /* _DATAMGR_H_ */
