#include "asciimap.h"

int custom_copy(char *copy_to, const char *copy_from)
{
	int i = 0;
	while (*copy_from != '\0')
	{
		*copy_to = *copy_from;
		copy_from++;
		copy_to++;
		i++;
	}
	*copy_to = '\0';
	i++;
	return i;
}

int custom_write_chars(char *src, char to_write, int amount)
{
	int i = 0;
	while (i < amount)
	{
		*src = to_write;
		src++;
		i++;
	}
	return i;
}

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
#ifdef _DEBUG
	printk("device_open(%p,%p)\n", inode, file);
#endif
	status.minor = inode->i_rdev >> 8;
	status.minor = inode->i_rdev & 0xFF;

	printk(
		"Device: %d.%d, busy: %d\n",
		status.major,
		status.minor,
		status.busy);

	if (status.busy)
		return -EBUSY;

	status.busy = true;

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
		put_user(*status.buf_ptr, buffer);
		status.buf_ptr++;
		buffer++;
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

/*

6. Implement the write() system call to write to the driver’s buffer the map data supplied.
It writes from the point where the current pointer was left off after the last read, write, or lseek.
The write updates the current buffer pointer within the driver and potentially the length.
Return an appropriate value and set errno appropriately if outside of the buffer’s bounds.

 */
static ssize_t device_write(file, buffer, length, offset)
struct file *file;
// 2. Maintain the current buffer pointer
const char *buffer; /* The buffer */
size_t length;		/* The length of the buffer */
loff_t *offset;		/* Our offset in the file */
{
	int written = 0;
	while (length > 0)
	{
		printk("asciimap::device_write() - Writing: [%c]\n", *buffer);
		int err = get_user(*status.buf_ptr, buffer);
		// TODO: Handle err
		status.buf_ptr++;
		buffer++;
		written++;
		length--;
	}
	if (written > status.map_size_in_bytes)
	{
		status.map_size_in_bytes = written;
	}
	// set the last char to null
	*status.buf_ptr = '\0';
#ifdef _DEBUG
	printk(
		"asciimap::device_write() - Length: [%d], Buf: [%s]\n",
		length,
		buffer);
#endif
	// Return an appropriate value and set errno appropriately if outside of the buffer’s bounds.
	return written;
}

/*
	7. Implement ioctl() to allow resetting the map back to its original default (all blank except the area pre-filled with the static initials map).
Write another ioctl option zero-out the entire buffer (with resetting the lengths and the pointer).
Write another ioctl option to check map for consistency and report of it isn’t, e.g.,
• byte length over width of the first line does not yield an integer
• the map contains non-printable ASCII characters (< 32 in ASCII codes), including null, within
its current length.
// */
static long device_ioctl(file, ioctl_num, ioctl_param)
struct file *file;
unsigned int ioctl_num; /* number and param for ioctl  */
unsigned long ioctl_param;
{
	return 0;
	switch (ioctl_num)
	{
	case IOCTL_RESET:
		// 7. Implement ioctl() to allow resetting the map back to its original default (all blank except the area pre-filled with the static initials map).
		status.map_size_in_bytes = custom_copy(status.buf, initials);
		status.buf_ptr = status.buf;
		break;
	case IOCTL_ZERO_OUT:
		// 7. Write another ioctl option zero-out the entire buffer (with resetting the lengths and the pointer).
		status.map_size_in_bytes = BSIZE_SQUARED;
		custom_write_chars(status.buf, '\0', BSIZE_SQUARED);
		status.buf_ptr = status.buf;
		break;
	case IOCTL_CHECK_CONSISTENCY:
		return 0;
		// 7. Write another ioctl option to check map for consistency and report of it isn’t, e.g.,
		// • byte length over width of the first line does not yield an integer
		// • the map contains non-printable ASCII characters (< 32 in ASCII codes), including null, within
		// its current length.
		char *ptr = status.buf;
		while (*ptr && *ptr != '\0')
		{
			// the map contains non-printable ASCII characters (< 32 in ASCII codes), including null, within its current length.
			ptr++;
		}
		break;
	}
	return 0;
}

/*
8.  Implement lseek() system call to allow internal pointer repositioning arbitrarily within your map’s buffer.
	Return an appropriate value and set errno appropriately if outside of the map’s bounds.
*/
static loff_t device_seek(struct file *file, loff_t offset, int whence)
{
	switch (whence)
	{
	case SEEK_SET:
		// 8. Implement lseek() system call to allow internal pointer repositioning arbitrarily within your map’s buffer.
		status.buf_ptr = status.buf + offset;
		break;
	case SEEK_CUR:
		// 8. Return an appropriate value and set errno appropriately if outside of the map’s bounds.
		if (status.buf_ptr + offset > status.buf + status.map_size_in_bytes)
		{
			return -2;
		}
		status.buf_ptr = status.buf_ptr + offset;
		break;
	case SEEK_END:
		status.buf_ptr = status.buf + status.map_size_in_bytes;
		break;
	}
	return 0;
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
	custom_write_chars(
		status.buf,
		'\0',
		BSIZE_SQUARED);
	status.map_size_in_bytes = custom_copy(
		status.buf,
		initials);
	return SUCCESS;
}

/* Cleanup - unregister the appropriate file from /proc */
void cleanup_module(void)
{
	unregister_chrdev(status.major, DEVICE_NAME);
}

MODULE_LICENSE("GPL");

/* EOF */
