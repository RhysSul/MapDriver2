#ifndef _ASCIIMAP_DEVICE_H
#define _ASCIIMAP_DEVICE_H

#define BSIZE (50 * 50)
/* The necessary header files */

/* Standard in kernel modules */
#include <linux/kernel.h> /* We're doing kernel work */
#include <linux/module.h> /* Specifically, a module */

/* For character devices */
#include <linux/fs.h> /* The character device
                             * definitions are here
                             */

#include <asm/uaccess.h> /* for put/get_user */

/* Return codes */
#define SUCCESS 0

/* Device Declarations **************************** */

/* The maximum length of the message from the device */
#define DRV_BUF_SIZE 80

/* The name for our device, as it will appear
 * in /proc/devices
 * 1. Call it map driver and the device /dev/asciimap
 */
#define DEVICE_NAME "/dev/asciimap"
#define BSIZE_SQUARED (BSIZE * BSIZE)
/*
 * Driver status structure
 */
typedef struct _driver_status
{
	uint map_size_in_bytes;

	/* Is the device open right now? Used to prevent
	 * concurent access into the same device
	 */
	bool busy;

	/* The message the device will give when asked */
	/*
		4. Define a BSIZE x BSIZE buffer
	*/
	char buf[BSIZE_SQUARED];

	/* How far did the process reading the message
	 * get? Useful if the message is larger than the size
	 * of the buffer we get to fill in device_read.
	 */
	char *buf_ptr;

	/* The major device number for the device.
	 */
	int major;

	/* The minor device number for the device.
	 */
	int minor;
} driver_status_t;

// 3. Define a static 50 x 50 char string in your driver with your team members initials.
const char *initials =
	" RS                                              \n"
	" JM                                              \n"
	" BP                                              \n"
	" NO                                              \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"                                                 \n";

/*
 * Driver funcitons' prototypes
 */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

/* Kernel module-related */

/* Module Declarations ***************************** */

/* This structure will hold the functions to be
 * called when a process does something to the device
 * we created. Since a pointer to this structure is
 * kept in the devices table, it can't be local to
 * init_module. NULL is for unimplemented functions.
 */
struct file_operations Fops =
	{
		.open = device_open,	  /* open */
		.read = device_read,	  /* read */
		.write = device_write,	  /* write */
		.release = device_release /* a.k.a. close */
#if 0
	.owner = NULL,   /* owner */
	.seek = NULL,   /* seek */
	.readdir = NULL,   /* readdir */
	.poll = NULL,   /* poll/select */
	.unlocked_ioctl = NULL,   /* ioctl */
	.mmap = NULL,   /* mmap */
	.flush = NULL,   /* flush */
#endif
};

int init_module(void);
void cleanup_module(void);

#endif /* _ASCIIMAP_DEVICE_H */
