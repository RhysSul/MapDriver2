

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
		return;
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
	printf("=== 1. Open the driver's device file (/dev/asciimap) ===\n");
	int driverFid, res;
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

	printf("=== 2. Read and display data from the driver's device file (which should be your static, initials map) using read() ===\n");
	printDriver(driverFid);

	/*
		3. Writes data to it using write(), which you will subsequently read using read (i.e., it should be apparent that the data you wrote ended up being stored in the driver).
	*/

	printf("=== 3. Writes data to it using write(), which you will subsequently read using read (i.e., it should be apparent that the data you wrote ended up being stored in the driver). ===\n");
	char writeBuffer[BUFSIZ];
	// TODO: Can we use memset here?
	char toWrite = 'a';
	for (int i = 0; i < BUFSIZ; i++)
	{
		writeBuffer[i] = toWrite;
	}
	lseek(driverFid, 0, SEEK_SET);
	int amount_written = write(driverFid, writeBuffer, BUFSIZ);
	printf("Wrote %d bytes to device file\n", amount_written);
	if (amount_written < 0)
	{
		printf("Error writing to device file %d\n", amount_written);
		return -1;
	}

	lseek(driverFid, 0, SEEK_SET);
	printDriver(driverFid);

	/*
		4. Uses all 3 ioctl() features on the driver's device file, demonstrating results from each of them
		(i.e., there should be a way to get the "Check Map" ioctl to succeed and to fail -- I want to see that both forms work).
	*/

	printf("=== 4. Uses all 3 ioctl() features on the driver's device file, demonstrating results from each of them ===\n");
	// ioctl reset
	printf("--- ioctl reset ---\n");
	lseek(driverFid, 0, SEEK_SET);
	res = ioctl(driverFid, IOCTL_RESET);
	if (res < 0)
	{
		printf("Error resetting driver %d\n", res);
		return -1;
	}
	printDriver(driverFid);
	// ioctl zero out

	printf("--- ioctl zero out ---\n");
	lseek(driverFid, 0, SEEK_SET);
	res = ioctl(driverFid, IOCTL_ZERO_OUT);
	if (res < 0)
	{
		printf("Error zeroing out driver %d\n", res);
		return -1;
	}
	printDriver(driverFid);

	// ioctl check consistency succeed
	printf("--- ioctl check consistency succeed ---\n");
	char smallWriteBuffer[2];
	smallWriteBuffer[0] = 'a';
	smallWriteBuffer[1] = 'a';
	lseek(driverFid, 0, SEEK_SET);
	write(driverFid, smallWriteBuffer, 2);
	lseek(driverFid, 0, SEEK_SET);
	printDriver(driverFid);
	lseek(driverFid, 0, SEEK_SET);
	res = ioctl(driverFid, IOCTL_CHECK_CONSISTENCY, 0);
	printf("Consistency check result: %d\n", res);

	// ioctl check consistency fail
	printf("--- ioctl check consistency fail ---\n");
	char smallWriteBufferInvalid[2];
	smallWriteBufferInvalid[0] = 21;
	smallWriteBufferInvalid[1] = 30;
	lseek(driverFid, 0, SEEK_SET);
	write(driverFid, smallWriteBufferInvalid, 2);
	lseek(driverFid, 0, SEEK_SET);
	printDriver(driverFid);
	lseek(driverFid, 0, SEEK_SET);
	res = ioctl(driverFid, IOCTL_CHECK_CONSISTENCY, 0);
	printf("Consistency check result: %d\n", res);

	/*
		5. Uses lseek() on the driver's device file, using all 3 forms of the "whence" parameter, and demonstrates that they work correctly.
	*/

	printf("=== 5. Uses lseek() on the driver's device file, using all 3 forms of the \"whence\" parameter, and demonstrates that they work correctly. ===\n");
	// Setup the buffer with an A at the beginning, Z at the end, and 0s in the middle

	char lseekBuffer[BUFSIZ];
	for (int i = 0; i < BUFSIZ; i++)
	{
		lseekBuffer[i] = '0';
	}
	lseekBuffer[0] = 'A';
	lseekBuffer[BUFSIZ - 1] = 'Z';
	lseekBuffer[30] = 'D';
	lseek(driverFid, 0, SEEK_SET);
	write(driverFid, lseekBuffer, BUFSIZ);
	printDriver(driverFid);

	char lseekBufferRead[BUFSIZ];
	// lseek SEEK_SET
	lseek(driverFid, 0, SEEK_SET);
	read(driverFid, lseekBufferRead, 1);
	printf("Read from SEEK_SET: %c\n", lseekBufferRead[0]);
	// lseek SEEK_CUR
	lseek(driverFid, 30, SEEK_SET);
	lseek(driverFid, 0, SEEK_CUR);
	read(driverFid, lseekBufferRead, 1);
	printf("Read from SEEK_CUR: %c\n", lseekBufferRead[0]);
	// lseek SEEK_END
	lseek(driverFid, -1, SEEK_END);
	read(driverFid, lseekBufferRead, 1);
	printf("Read from SEEK_END: %c\n", lseekBufferRead[0]);
}

/* EOF */
