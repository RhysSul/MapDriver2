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
#include <linux/ioctl.h>

#include <asm/uaccess.h> /* for put/get_user */

/* Return codes */
#define SUCCESS 0

#define IOCTL_RESET = 0
#define IOCTL_ZERO_OUT = 1
#define IOCTL_CHECK_CONSISTENCY = 2
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
	" RS                                            JM\n"
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
	"                                                 \n"
	"                                                 \n"
	"                                                 \n"
	"BP                                             NO\n";

/*
 * Driver funcitons' prototypes
 */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static loff_t device_seek(struct file *, loff_t, int);
static long device_ioctl(struct file *, unsigned int, unsigned long);

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
		.open = device_open,			/* open */
		.read = device_read,			/* read */
		.write = device_write,			/* write */
		.llseek = device_seek,			/* seek */
		.unlocked_ioctl = device_ioctl, /* ioctl */
		.release = device_release		/* a.k.a. close */
};

int init_module(void);
void cleanup_module(void);

#endif /* _ASCIIMAP_DEVICE_H */
