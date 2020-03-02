#ifndef DUMMY_H
#define DUMMY_H

struct dummy_device {
	struct platform_device *pdev;
	struct miscdevice mdev;
	void __iomem *mmio;
};

/* Retrieves Dummy device instance on file operations */
static struct dummy_device *
to_dummy_device(struct file *file);
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
static int
dummy_write(struct file *file, const char __user *data, size_t size,
	    loff_t * offset);

#endif /* DUMMY_H */
