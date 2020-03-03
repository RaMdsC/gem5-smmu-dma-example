#ifndef DUMMY_H
#define DUMMY_H

#include "linux/miscdevice.h"

enum Offset {
	BASE	= 0x00,
	RANGE	= 0x08,
	CONTROL	= 0x10,
	STATUS	= 0x11
};

struct dummy_device {
	struct platform_device *pdev;
	struct miscdevice mdev;
	void __iomem *mmio;
        struct mutex lock;
};

/* Retrieves Dummy device instance on file operations */
static struct dummy_device *
to_dummy_device(struct file *filp);
/* Registers a Dummy device from DTB */
static int
dummy_probe(struct platform_device *pdev);
/* Destroys a Dummy device */
static int
dummy_remove(struct platform_device *pdev);
/*
 * Handles writes to /dev/dummyN. Initiates DMA operations
 * Usage: echo "0xbeef 1024" > /dev/dummyN
 */
static ssize_t
dummy_write(struct file *filp, const char __user *buffer, size_t count,
	    loff_t *offset);

#endif /* DUMMY_H */
