#include <stdio.h>
#include "config.h"


#define FILE_ERROR(fp,error_msg) 	do { \
					  if ((fp)==NULL) { \
					    printf("%s\n",(error_msg)); \
					    exit(EXIT_FAILURE); \
					  }	\
					} while(0)

int main(int argc, const char *argv[])
{
	FILE *fp_bin;
	sensor_data_t data;

	fp_bin = fopen("sensor_data_recv", "r");
  FILE_ERROR(fp_bin,"Couldn't create sensor_data\n");  
	while (1)
	{
		if (feof(fp_bin) != 0)
			break;
		fread(&data.id, sizeof(data.id), 1, fp_bin);
		fread(&data.value, sizeof(data.value), 1, fp_bin);
		fread(&data.ts, sizeof(data.ts), 1, fp_bin);
		printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value, (long int)data.ts);
	}
	fclose(fp_bin);
	return 0;
}
