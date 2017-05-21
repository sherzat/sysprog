//#include <stdio.h>
#include <stdlib.h>
#include "./lib/dplist.h"
#include "datamgr.h"
#include "config.h"

#define FILE_ERROR(fp,error_msg) 	do { \
					  if ((fp)==NULL) { \
					    printf("%s\n",(error_msg)); \
					    exit(EXIT_FAILURE); \
					  }	\
					} while(0)

int main(int argc, const char *argv[])
{
	FILE *fp_text, *fp_bin;
  fp_text = fopen("room_sensor.map", "r");
  FILE_ERROR(fp_text,"Couldn't create room_sensor.map\n");
  fp_bin = fopen("sensor_data", "r");
  FILE_ERROR(fp_bin,"Couldn't create sensor_data\n");  

	datamgr_parse_sensor_files(fp_text,fp_bin);
	
//	room_id_t room_id = datamgr_get_room_id(15);
//	sensor_value_t avg = datamgr_get_avg(1);
//	sensor_ts_t last_modified =	datamgr_get_last_modified(1);
//	int total_sensor = datamgr_get_total_sensors();
//	total_sensor = datamgr_get_total_sensors();
	datamgr_free();

	fclose(fp_text);
	fclose(fp_bin);
	
	return 0;
}
