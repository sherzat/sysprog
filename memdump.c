#include <stdio.h>
#include "memdump.h"
#include <ctype.h>
void memdump( byte_t *ptr, long size ) {
  int i;
  
  // print table header
  printf("\n%-*s", ADDR_COLUMN_WIDTH, "Address");
  printf("%s", COLUMN_SEPARATOR);
  printf("%-*s", BYTE_COLUMN_WIDTH, "Bytes");
  printf("%s", COLUMN_SEPARATOR);
  printf("%-*s\n", BYTES_A_ROW, "Chars");
  
  for( i= 0; i < ADDR_COLUMN_WIDTH; i++)
    printf("-");
  printf("%s", COLUMN_SEPARATOR);
  for( i= 0; i < BYTE_COLUMN_WIDTH; i++)
    printf("-");
  printf("%s", COLUMN_SEPARATOR);
  for( i= 0; i < BYTES_A_ROW; i++)    // number of chars is char-column is exactly BYTES_A_ROW
    printf("-");
  printf("\n");
  
  /*
   * here comes your code !!!!!!!!!
   * 
   * */
 int j; 
	for (i = 0; i < (size/BYTES_A_ROW) ; i++)
	{
		printf("\n%-*p", ADDR_COLUMN_WIDTH, ptr+i*BYTES_A_ROW);
		printf("%s", COLUMN_SEPARATOR);
		for (j = 0; j < BYTES_A_ROW; j++)
		{
			printf("%02x", *(ptr+i*BYTES_A_ROW+j));
			printf("%s", COLUMN_SEPARATOR);
		}
 		printf("%s", COLUMN_SEPARATOR);
		for (j = 0; j < BYTES_A_ROW; j++)
		{
			printf("%c", isprint(*(ptr+i*BYTES_A_ROW+j))?(*(ptr+i*BYTES_A_ROW+j)):'.');
		}
	}

	if (size % BYTES_A_ROW != 0)
	{
		printf("\n%-*p", ADDR_COLUMN_WIDTH, ptr+i*BYTES_A_ROW);
		printf("%s", COLUMN_SEPARATOR);
		for (j = 0; j < BYTES_A_ROW; j++)
		{
			if (size%BYTES_A_ROW>j)
			{
				printf("%02x", *(ptr+i*BYTES_A_ROW+j));
				printf("%s", COLUMN_SEPARATOR);
			}
			else
				printf("%-3s", " ");
		}

 		printf("%s", COLUMN_SEPARATOR);

		for (j = 0; j < BYTES_A_ROW; j++)
		{
			if (size%BYTES_A_ROW>j)
			{
				printf("%c", isprint(*(ptr+i*BYTES_A_ROW+j))?(*(ptr+i*BYTES_A_ROW+j)):'.');
			}
			else
				printf("%-3s", " ");
		}
	}
	printf("\n");
}
