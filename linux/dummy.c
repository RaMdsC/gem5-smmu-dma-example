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
to_dummy_device(struct file *file) {
	struct miscdevice *miscdev = file->private_data;

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

static int
dummy_write(struct file *file, const char __user *data, size_t size,
	    loff_t * offset) {
	struct dummy_device *dummy_dev;

	dummy_dev = to_dummy_device(file);
	// TODO: parse and program DMA
}

module_platform_driver(dummy_drv);

MODULE_LICENSE("GPL");
