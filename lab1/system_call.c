#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
	//system("clear");
	system("ls > files.txt");
	system("sort files.txt | more");
	return 0;
}
