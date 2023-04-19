#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/ioctl.h>

#include "common.h"

int main(argc, argv)
int argc;
char *argv[];
{
	char buf[BSIZE];
	int fd, i, j, n;
	fd = open("/dev/asciimap", O_RDWR);
	if (fd != 0)
	{
		printf("Error opening device file\n");
		return -1;
	}
}

/* EOF */
