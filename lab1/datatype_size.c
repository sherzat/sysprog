#include <stdio.h>
#include <stdlib.h>
int main(int argc, const char *argv[])
{
	printf("sizeof int: %lu\n", sizeof(int));
	printf("sizeof char: %lu\n", sizeof(char));
	printf("sizeof float: %lu\n", sizeof(float));
	printf("sizeof double: %lu\n", sizeof(double));
	printf("sizeof void: %lu\n", sizeof(void));
	printf("sizeof int pointer: %lu\n", sizeof(int *));
	printf("sizeof float pointer: %lu\n", sizeof(float *));
	printf("sizeof double pointer: %lu\n", sizeof(double *));
	printf("sizeof void pointer: %lu\n", sizeof(void *));
	printf("sizeof long int: %lu\n", sizeof(long int));
	printf("sizeof short int: %lu\n", sizeof(short int));
	
	char a = 128;
	printf("char a = 128, actual output value: %d\n", a);
	a = -98;
	printf("char a = -98, actual output value: %d\n", a);
	return 0;
}
