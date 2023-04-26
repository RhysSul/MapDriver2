

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/ioctl.h>

#include "common.h"

void printDriver(int driverFid)
{
	char buffer[BUFSIZ];
	int amount_read = read(driverFid, buffer, BUFSIZ);
	if (amount_read < 0)
	{
		printf("Error reading from device file\n");
		return -1;
	}
	printf("Read %d bytes from device file\n", amount_read);
	printf("%s\n", buffer);
}

/*
You should have a test program that:

Opens the driver's device file ("/dev/asciimap")
Reads and displays data from the driver's device file (which should be your static, initials map) using read()
Writes data to it using write(), which you will subsequently read using read (i.e., it should be apparent that the data you wrote ended up being stored in the driver).
Uses all 3 ioctl() features on the driver's device file, demonstrating results from each of them (i.e., there should be a way to get the "Check Map" ioctl to succeed and to fail -- I want to see that both forms work).
Uses lseek() on the driver's device file, using all 3 forms of the "whence" parameter, and demonstrates that they work correctly.
*/
int main(argc, argv)
int argc;
char *argv[];
{
	// 1. Open the driver's device file ("/dev/asciimap")
	int driverFid;
	driverFid = open("/dev/asciimap", O_RDWR);
	if (driverFid < 0)
	{
		printf("Error opening device file\n", driverFid);
		return -1;
	}
	printf("Opened device file\n");
	/*
		2. Read and display data from the driver's device file (which should be your static, initials map) using read()
	*/
	printDriver(driverFid);

	// set pointer to beginning of file
	lseek(driverFid, 0, SEEK_SET);

	/*
		3. Writes data to it using write(), which you will subsequently read using read (i.e., it should be apparent that the data you wrote ended up being stored in the driver).
	*/
	char writeBuffer[BUFSIZ];
	// TODO: Can we use memset here?
	char toWrite = 'a';
	for (int i = 0; i < BUFSIZ; i++)
	{
		writeBuffer[i] = toWrite;
	}
	int amount_written = write(driverFid, writeBuffer, BUFSIZ);
	printf("Wrote %d bytes to device file\n", amount_written);
	if (amount_written < 0)
	{
		printf("Error writing to device file %d\n", amount_written);
		return -1;
	}

	// Set pointer to beginning of file
	lseek(driverFid, 0, SEEK_SET);

	printDriver(driverFid);

	/*
		4. Uses all 3 ioctl() features on the driver's device file, demonstrating results from each of them
		(i.e., there should be a way to get the "Check Map" ioctl to succeed and to fail -- I want to see that both forms work).
	*/

	// ioctl reset
	printf("Resetting driver\n");
	ioctl(driverFid, IOCTL_RESET);
	printDriver(driverFid);
	// ioctl zero out

	printf("Zeroing out driver\n");
	icotl(driverFid, IOCTL_ZERO_OUT);
	printDriver(driverFid);

	// ioctl check consistency succeed

	/*
		5. Uses lseek() on the driver's device file, using all 3 forms of the "whence" parameter, and demonstrates that they work correctly.
	*/
}

/* EOF */
