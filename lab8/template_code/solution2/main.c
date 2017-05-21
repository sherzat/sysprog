#include <stdio.h>
#include <stdlib.h>
#include "connmgr.h"
#include "config.h"

int main(int argc, const char *argv[])
{
	if (argc < 2)
	{
		puts("not enough command line argument, set port number");
		exit(EXIT_FAILURE);
	}

	int port_number = atoi(argv[1]);
	printf("port number = %d\n", port_number);

	connmgr_listen(port_number);
	connmgr_free();
	return 0;
}
