#include "asciimap.h"

/* Driver's Status is kept here */
static driver_status_t status =
	{
		.map_size_in_bytes = 0,
		.busy = false,
		.buf_ptr = NULL,
		.buf = {0},
		.major = -1,
		.minor = -1,
};

/* This function is called whenever a process
 * attempts to open the device file
 */
static int device_open(inode, file)
struct inode *inode;
struct file *file;
{
	static int counter = 0;

#ifdef _DEBUG
	printk("device_open(%p,%p)\n", inode, file);
#endif

	/* This is how you get the minor device number in
	 * case you have more than one physical device using
	 * the driver.
	 */
	status.minor = inode->i_rdev >> 8;
	status.minor = inode->i_rdev & 0xFF;

	printk(
		"Device: %d.%d, busy: %d\n",
		status.major,
		status.minor,
		status.busy);

	/* We don't want to talk to two processes at the
	 * same time
	 */
	if (status.busy)
		return -EBUSY;

	/* If this was a process, we would have had to be
	 * more careful here.
	 *
	 * In the case of processes, the danger would be
	 * that one process might have check busy
	 * and then be replaced by the schedualer by another
	 * process which runs this function. Then, when the
	 * first process was back on the CPU, it would assume
	 * the device is still not open.
	 *
	 * However, Linux guarantees that a process won't be
	 * replaced while it is running in kernel context.
	 *
	 * In the case of SMP, one CPU might increment
	 * busy while another CPU is here, right after
	 * the check. However, in version 2.0 of the
	 * kernel this is not a problem because there's a lock
	 * to guarantee only one CPU will be kernel module at
	 * the same time. This is bad in  terms of
	 * performance, so version 2.2 changed it.
	 */

	status.busy = true;

	/* Initialize the message. */
	sprintf(
		status.buf,
		"If I told you once, I told you %d times - %s",
		counter++,
		"Hello, world\n");

	/* The only reason we're allowed to do this sprintf
	 * is because the maximum length of the message
	 * (assuming 32 bit integers - up to 10 digits
	 * with the minus sign) is less than DRV_BUF_SIZE, which
	 * is 80. BE CAREFUL NOT TO OVERFLOW BUFFERS,
	 * ESPECIALLY IN THE KERNEL!!!
	 */

	status.buf_ptr = status.buf;

	return SUCCESS;
}

/* This function is called when a process closes the
 * device file.
 */
static int device_release(inode, file)
struct inode *inode;
struct file *file;
{
#ifdef _DEBUG
	printk("device_release(%p,%p)\n", inode, file);
#endif

	/* We're now ready for our next caller */
	status.busy = false;

	return SUCCESS;
}

/*
	5.  Implement the read() system call to return the map data within appropriate size and bounds and map limits.
	Read for now is a singleton read (no concurrent reads are allowed).
	If you stored your map as a contiguous set of lines (one-dimensional array) delimited by \n and
	terminated by null, that’s basically what you return per size requested/available.
	Attempts to read beyond the last line, would simply return 0 bytes.
	The read updates the current buffer pointer within the driver.
 */
static ssize_t device_read(file, buffer, length, offset)
struct file *file;
char *buffer;	/* The buffer to fill with data */
size_t length;	/* The length of the buffer */
loff_t *offset; /* Our offset in the file */
{
	/* Number of bytes actually written to the buffer */
	int bytes_read = 0;

	/* Actually put the data into the buffer */
	while (length > 0)
	{
		// attempts to read beyond the last line, would simply return 0 bytes
		if (*status.buf_ptr == '\0')
		{
			return 0;
		}
		char ltr = *status.buf_ptr;
		put_user(ltr, buffer++);
		*status.buf_ptr++;

		length--;
		bytes_read++;
	}

#ifdef _DEBUG
	printk(
		"asciimap::device_read() - Read %d bytes, %d left\n",
		bytes_read,
		length);
#endif

	/* Read functions are supposed to return the number
	 * of bytes actually inserted into the buffer
	 */
	return bytes_read;
}

/* This function is called when somebody tries to write
 * into our device file.
 */
static ssize_t device_write(file, buffer, length, offset)
struct file *file;
// 2. Maintain the current buffer pointer
const char *buffer; /* The buffer */
size_t length;		/* The length of the buffer */
loff_t *offset;		/* Our offset in the file */
{
	int nbytes = 0;

#ifdef _DEBUG
	printk(
		"asciimap::device_write() - Length: [%d], Buf: [%s]\n",
		length,
		buffer);
#endif
	return nbytes;
}

static int copy(const char *src, char *goal)
{
	int i = 0;
	while (*src != '\0')
	{
		*goal = *src;
		goal++;
		src++;
		i++;
	}
	*goal = '\0';
	i++;
	return i;
}

static int write_chars(const char *src, char to_write, int amount)
{
	for (int i = 0; i < amount; i++)
	{
		*src = to_write;
		src++;
	}
}

/* Initialize the module - Register the character device */
int init_module(void)
{
	/* Register the character device (atleast try) */
	status.major = register_chrdev(
		0,
		DEVICE_NAME,
		&Fops);

	/* Negative values signify an error */
	if (status.major < 0)
	{
		printk(
			"Sorry, registering the ASCIIMap device failed with %d\n",
			status.major);

		return status.major;
	}

	printk(
		"Registeration is a success. The major device number is %d.\n",
		status.major);

	printk(
		"If you want to talk to the device driver,\n"
		"you'll have to create a device file. \n"
		"We suggest you use:\n\n"
		"mknod %s c %d <minor>\n\n"
		"You can try different minor numbers and see what happens.\n",
		DEVICE_NAME,
		status.major);
	// 4. Initial set equal to the static map above and the region beyond the map set to 0
	write_chars(
		status.map,
		'0',
		BSIZE_SQUARED);
	status.map_size_in_bytes = copy(
		initials,
		status.buf);
	return SUCCESS;
}

/* Cleanup - unregister the appropriate file from /proc */
void cleanup_module(void)
{
	unregister_chrdev(status.major, DEVICE_NAME);
}

MODULE_LICENSE("GPL");

/* EOF */
