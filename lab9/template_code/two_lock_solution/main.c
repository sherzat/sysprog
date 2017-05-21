#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>

#include "config.h"
#include "sbuffer.h"


#define FILE_ERROR(fp,error_msg) 	do { \
					  if ((fp)==NULL) { \
					    printf("%s\n",(error_msg)); \
					    exit(EXIT_FAILURE); \
					  }	\
					} while(0)
void *writer(void *arg);
void *reader();

sbuffer_t *buffer;

int main(int argc, const char *argv[])
{
	FILE *fp_bin;
	pthread_t writer1, writer2, writer3, writer4, writer5, writer6, reader1, reader2, reader3, 
						reader4, reader5, reader6;
	
	fp_bin = fopen("sensor_data_recv", "r");
  FILE_ERROR(fp_bin,"Couldn't create sensor_data\n");  

	if (sbuffer_init(&buffer) != SBUFFER_SUCCESS) exit(EXIT_FAILURE);
	
	pthread_create(&writer1, NULL, writer, fp_bin);
	pthread_create(&writer2, NULL, writer, fp_bin);
	pthread_create(&writer3, NULL, writer, fp_bin);
	pthread_create(&writer4, NULL, writer, fp_bin);
	pthread_create(&writer5, NULL, writer, fp_bin);
	pthread_create(&writer6, NULL, writer, fp_bin);
	pthread_create(&reader1, NULL, reader, NULL);
	pthread_create(&reader2, NULL, reader, NULL);
	pthread_create(&reader3, NULL, reader, NULL);
	pthread_create(&reader4, NULL, reader, NULL);
	pthread_create(&reader5, NULL, reader, NULL);
	pthread_create(&reader6, NULL, reader, NULL);

	pthread_join(writer1, NULL);
	pthread_join(writer2, NULL);
	pthread_join(writer3, NULL);
	pthread_join(writer4, NULL);
	pthread_join(writer5, NULL);
	pthread_join(writer6, NULL);
	pthread_join(reader1, NULL);
	pthread_join(reader2, NULL);
	pthread_join(reader3, NULL);
	pthread_join(reader4, NULL);
	pthread_join(reader5, NULL);
	pthread_join(reader6, NULL);
//	puts("all joined");
	fclose(fp_bin);
	if (sbuffer_free(&buffer) != SBUFFER_SUCCESS) exit(EXIT_FAILURE);
	return 0;
}

void *writer(void * arg)
{
	FILE *fp_bin = (FILE *)arg;
	sensor_data_t data;
	while (1)
	{
		flockfile(fp_bin);// To read 3 data values continuously from the file without interruption by other threads.
		fread(&data.id, sizeof(data.id), 1, fp_bin);
		fread(&data.value, sizeof(data.value), 1, fp_bin);
		fread(&data.ts, sizeof(data.ts), 1, fp_bin);
		if (feof(fp_bin) != 0)
		{
			funlockfile(fp_bin);
			break;
		}
		sbuffer_insert(buffer, &data);
		funlockfile(fp_bin);
	//	printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value, (long int)data.ts);
	}

}

void *reader()
{
	sensor_data_t data;
	int retval;
//	sleep(1);
	while (1)
	{
//		puts("\nblock");
		retval = sbuffer_remove(buffer,&data, 5);
//	puts("unblock");

//		printf("retval = %d\n", retval);
		if(retval == SBUFFER_NO_DATA)
			break;
		printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", 
				data.id, data.value, (long int)data.ts);
	}
}
