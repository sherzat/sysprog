#include <stdio.h>

void swap_pointers(int **p,int **q);

int main(int argc, const char *argv[])
{
	int a = 1;
	int b = 2;

	int *p = &a;
	int *q = &b;
	printf("address of p = %p\taddress of q = %p\n", p, q);

	swap_pointers(&p,&q);

	printf("address of p = %p\taddress of q = %p\n", p, q);

	return 0;
}

void swap_pointers(int **p,int **q)
{
	int *temp = NULL;
	temp = *p;
	*p = *q;
	*q = temp;
}
