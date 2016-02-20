#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX 35
#define MIN -10
#define FREQUENCY 5

int main(int argc, const char *argv[])
{
	srand(time(NULL));
	int rand_value;
	double delta = MAX - MIN ;
	double temp;
	while (1)
	{
		rand_value = rand();
		temp = rand_value / (RAND_MAX * 1.0);
		temp = temp * delta + MIN;	
		printf("Temperature = %2.2f @ ", temp);
		fflush(stdout);
		system("date");
		sleep(FREQUENCY);
	}
	return 0;
}


