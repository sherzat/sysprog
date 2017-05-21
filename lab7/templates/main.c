#include <sqlite3.h>
#include <stdio.h>
#include "config.h"

#define FILE_ERROR(fp,error_msg) 	do { \
					  if ((fp)==NULL) { \
					    printf("%s\n",(error_msg)); \
					    exit(EXIT_FAILURE); \
					  }	\
					} while(0)

int callback(void * para, int col_number, char ** value, char **col_name)
{
	for (int i = 0; i < col_number; i++)
	{
		printf("%s = %s  ", col_name[i], value[i] ? value[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, const char *argv[])
{
	sqlite3 *db;
	db = init_connection(1);
	FILE *fp_bin;

  fp_bin = fopen("sensor_data", "r");
  FILE_ERROR(fp_bin,"Couldn't create sensor_data\n");  
	
	insert_sensor_from_file(db, fp_bin);
	puts("*******************find_sensor_all*************************************");
	find_sensor_all(db, callback);
	puts("*******************find_sensor_by_valu*********************************");
	find_sensor_by_value(db, 16.75, callback);
	find_sensor_by_value(db, 15, callback);
	find_sensor_by_value(db, 15.1641, callback);
	puts("*******************find_sensor_exceed_value****************************");
	find_sensor_exceed_value(db, 25, callback);
	puts("*******************find_sensor_by_timestam*****************************");
	find_sensor_by_timestamp(db, 1461188244, callback);
	find_sensor_by_timestamp(db, 1461188124, callback);
	puts("*******************find_sensor_after_timestamp*************************");
	find_sensor_after_timestamp(db, 1461189204, callback);
	fclose(fp_bin);
	disconnect(db);
	return 0;
}
