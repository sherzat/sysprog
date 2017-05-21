#include <stdio.h>


typedef	unsigned char byte_t;
#define DATA_SIZE 123

#define BYTES_A_ROW         ((int)10)
#define ADDR_COLUMN_WIDTH   ((int)(2*sizeof(void *)))  //char size of an address = num bytes of a pointer * 2 char; (1 byte = hex 2 chars)
#define BYTE_COLUMN_WIDTH   ((int)(BYTES_A_ROW * 3)) // 1 bytes is 2 hex chars + some spaces
#define COLUMN_SEPARATOR    " "

int main(int argc, const char *argv[])
{
	int i,j;
	byte_t data[DATA_SIZE];
	printf("\n%-*s", ADDR_COLUMN_WIDTH, "Address");
	printf("%s", COLUMN_SEPARATOR);
	printf("%-*s", BYTE_COLUMN_WIDTH, "Bytes");
	printf("%s", COLUMN_SEPARATOR);
	printf("%-*s\n", BYTES_A_ROW, "Chars");
	
	for (i = 0; i < (DATA_SIZE/BYTES_A_ROW) ; i++)
	{
		printf("\n%-*p", ADDR_COLUMN_WIDTH, data+i*BYTES_A_ROW);
		printf("%s", COLUMN_SEPARATOR);
		for (j = 0; j < BYTES_A_ROW; j++)
		{
			printf("%2x", *(data+i*BYTES_A_ROW+j));
			printf("%s", COLUMN_SEPARATOR);
		}
 		printf("%s", COLUMN_SEPARATOR);
		for (j = 0; j < BYTES_A_ROW; j++)
		{
			printf("%c", isprint((byte_t) data+i*BYTES_A_ROW+j)?((byte_t) data+i*BYTES_A_ROW+j):'.');
			printf("%s", COLUMN_SEPARATOR);
		}
	}

	if (DATA_SIZE % BYTES_A_ROW != 0)
	{
		printf("\n%-*p", ADDR_COLUMN_WIDTH, data+i*BYTES_A_ROW);
		printf("%s", COLUMN_SEPARATOR);
		for (j = 0; j < BYTES_A_ROW; j++)
		{
			if (DATA_SIZE%BYTES_A_ROW>j)
			{
				printf("%2x", *(data+i*BYTES_A_ROW+j));
				printf("%s", COLUMN_SEPARATOR);
			}
			else
				printf("%-3s", " ");
		}

 		printf("%s", COLUMN_SEPARATOR);

		for (j = 0; j < BYTES_A_ROW; j++)
		{
			if (DATA_SIZE%BYTES_A_ROW>j)
			{
				printf("%c", isprint((byte_t) data+i*BYTES_A_ROW+j)?((byte_t) data+i*BYTES_A_ROW+j):'.');
				printf("%s", COLUMN_SEPARATOR);
			}
			else
				printf("%-3s", " ");
		}
		printf("\n");
	 }
	 return 0;
}
