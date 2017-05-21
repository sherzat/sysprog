#include <stdio.h>


typedef	unsigned char byte_t;
#define DATA_SIZE 10


int main(int argc, const char *argv[])
{
	 byte_t data[DATA_SIZE];
	 for (int i = 0; i < DATA_SIZE; i++)
	 {
		 printf("%p %x %c\n", data+i,(byte_t) data+i,(byte_t) data+i);
	 }
	 return 0;
}
