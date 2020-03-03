#include "dummy.h"

#include "linux/dma-mapping.h"
#include "linux/fs.h"
#include "linux/io.h"
#include "linux/mod_devicetable.h"
#include "linux/module.h"
#include "linux/platform_device.h"
#include "linux/slab.h"

static const struct of_device_id dummy_match_table[] = {
	{ .compatible = "arm,dummy" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, dummy_match_table);

static struct platform_driver dummy_drv = {
	.probe = dummy_probe,
	.remove = dummy_remove,
	.driver = {
		.name = "dummy",
		.of_match_table = dummy_match_table,
		.owner = THIS_MODULE
	}
};

static const struct file_operations dummy_fops = {
	.owner = THIS_MODULE,
	.write = dummy_write
};

static struct dummy_device *
to_dummy_device(struct file *filp) {
	struct miscdevice *miscdev = filp->private_data;

	return container_of(miscdev, struct dummy_device, mdev);
}

static int
dummy_probe(struct platform_device *pdev) {
	int err;
	struct dummy_device *dummy_dev;
	struct resource *res;

	dummy_dev = kzalloc(sizeof(*dummy_dev), GFP_KERNEL);
	if (!dummy_dev) return -ENOMEM;

	platform_set_drvdata(pdev, dummy_dev);
	dummy_dev->pdev = pdev;
	dummy_dev->mdev.minor = MISC_DYNAMIC_MINOR;
	dummy_dev->mdev.name = "dummy";
	dummy_dev->mdev.fops = &dummy_fops;
	dummy_dev->mdev.parent = NULL;

	err = dma_set_mask_and_coherent(&(pdev->dev), DMA_BIT_MASK(64));
	if (err) return err;
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) return -EIO;
	dummy_dev->mmio = devm_ioremap_resource(&(pdev->dev), res);
	if (IS_ERR(dummy_dev->mmio)) return PTR_ERR(dummy_dev->mmio);
	mutex_init(&(dummy_dev->lock));

	err = misc_register(&(dummy_dev->mdev));
	if (err) return err;

	return 0;
}

static int
dummy_remove(struct platform_device *pdev) {
	struct dummy_device *dummy_dev;

	dummy_dev = platform_get_drvdata(pdev);
	misc_deregister(&(dummy_dev->mdev));
	devm_iounmap(&(pdev->dev), dummy_dev->mmio);
	kfree(dummy_dev);

	return 0;
}

static ssize_t
dummy_write(struct file *filp, const char __user *buffer, size_t count,
	    loff_t *offset) {
	int err;
	struct dummy_device *dummy_dev;
	unsigned long long range;
	dma_addr_t dev_addr;
        void *cpu_addr;
	uint8_t completed;

	err = kstrtoull_from_user(buffer, count, 10, &range);
	if (err) return err;
	dummy_dev = to_dummy_device(filp);
	mutex_lock(&(dummy_dev->lock));
	// Allocate and map buffer
	cpu_addr = dma_alloc_coherent(&(dummy_dev->pdev->dev), range,
				      &dev_addr, GFP_KERNEL);
	iowrite64(dev_addr, dummy_dev->mmio + BASE);
	iowrite64(range, dummy_dev->mmio + RANGE);
	// Initiate DMA operation
	iowrite8(1, dummy_dev->mmio + CONTROL);
	// Wait for completion
	do {
		completed = ioread8(dummy_dev->mmio + STATUS);
	} while (!completed);
	// Acknowledge completion
	iowrite8(0, dummy_dev->mmio + STATUS);
	dma_free_coherent(&(dummy_dev->pdev->dev), range, cpu_addr, dev_addr);
	mutex_unlock(&(dummy_dev->lock));
	*offset = count;

	return count;
}

module_platform_driver(dummy_drv);

MODULE_LICENSE("GPL");
