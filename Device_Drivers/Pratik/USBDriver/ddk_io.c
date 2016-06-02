#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/usb.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#include "ddk_io.h"

static struct ddk_device ddk_dev; // Need to be persistent

#ifdef ENABLE_FILE_OPS
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

static int ddk_open(struct inode *i, struct file *f)
{
	return 0;
}
static int ddk_close(struct inode *i, struct file *f)
{
	return 0;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int ddk_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long ddk_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
	Register r;
	int val, ind;
	char buf;
	int retval;

	if (copy_from_user(&r, (Register *)arg, sizeof(Register)))
	{
		return -EFAULT;
	}
	ind = r.id;
	switch (cmd)
	{
		case DDK_REG_GET:
			/* Control IN */
			retval = usb_control_msg(ddk_dev.device, usb_rcvctrlpipe(ddk_dev.device, 0),
						CUSTOM_RQ_GET_REGISTER, USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
						0, ind, &buf, sizeof(buf), 0);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			r.val = buf;
			if (copy_to_user((Register *)arg, &r, sizeof(Register)))
			{
				return -EFAULT;
			}
			break;
		case DDK_REG_SET:
			/* Control OUT */
			val = r.val;
			retval = usb_control_msg(ddk_dev.device, usb_sndctrlpipe(ddk_dev.device, 0),
						CUSTOM_RQ_SET_REGISTER, USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
						val, ind, NULL, 0, 0);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			break;
		default:
			return -EINVAL;
			break;
	}
	return 0;
}

static struct file_operations fops =
{
	.open = ddk_open,
	.release = ddk_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
	.ioctl = ddk_ioctl,
#else
	.unlocked_ioctl = ddk_ioctl,
#endif
};

static int char_register_dev(struct usb_interface *interface, struct usb_class_driver *class)
{
	int ret;
	struct device *dev_ret;

	if ((ret = alloc_chrdev_region(&dev, 0, 1, "ddk_io")) < 0)
	{
		return ret;
	}
	printk(KERN_INFO "(Major, Minor): (%d, %d)\n", MAJOR(dev), MINOR(dev));
	interface->minor = MINOR(dev);

	cdev_init(&c_dev, class->fops);
	if ((ret = cdev_add(&c_dev, dev, 1)) < 0)
	{
		return ret;
	}

	if (IS_ERR(cl = class_create(THIS_MODULE, "usb")))
	{
		cdev_del(&c_dev);
		unregister_chrdev_region(dev, 1);
		return PTR_ERR(cl);
	}
	if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, class->name, 0)))
	{
		class_destroy(cl);
		cdev_del(&c_dev);
		unregister_chrdev_region(dev, 1);
		return PTR_ERR(dev_ret);
	}
	return 0;
}

static void char_deregister_dev(struct usb_interface *interface, struct usb_class_driver *class)
{
	device_destroy(cl, dev);
	class_destroy(cl);
	cdev_del(&c_dev);
	unregister_chrdev_region(dev, 1);
}
#endif

static int ddk_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	int i;
	int retval;

	iface_desc = interface->cur_altsetting;
	printk(KERN_INFO "DDK USB i/f %d now probed: (%04X:%04X)\n",
		iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
	printk(KERN_INFO "ID->bNumEndpoints: %02X\n", iface_desc->desc.bNumEndpoints);
	printk(KERN_INFO "ID->bInterfaceClass: %02X\n", iface_desc->desc.bInterfaceClass);

	/* Set up the endpoint information. Assuming there is 1 in & 1 out */
	memset(&ddk_dev, 0, sizeof(ddk_dev));
	for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
	{
		endpoint = &iface_desc->endpoint[i].desc;

		printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n", i, endpoint->bEndpointAddress);
		printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n", i, endpoint->bmAttributes);
		printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n", i, endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);
	}

	ddk_dev.device = interface_to_usbdev(interface);

#ifdef ENABLE_FILE_OPS
	ddk_dev.class.name = "usb/ddk_io%d";
	ddk_dev.class.fops = &fops;
	retval = char_register_dev(interface, &ddk_dev.class);
	if (retval)
	{
		/* Something prevented us from registering this driver */
		printk(KERN_ERR "Not able to get a minor for this device.\n");
		return retval;
	}
	else
	{
		printk(KERN_INFO "Minor obtained: %d\n", interface->minor);
	}
#else
	interface->minor = iface_desc->desc.bInterfaceNumber;
#endif

	return 0;
}

static void ddk_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "Releasing Minor: %d\n", interface->minor);

#ifdef ENABLE_FILE_OPS
	/* Give back our minor */
	char_deregister_dev(interface, &ddk_dev.class);
#endif

	printk(KERN_INFO "DDK USB i/f %d now disconnected\n",
			interface->cur_altsetting->desc.bInterfaceNumber);
}

/* Table of devices that work with this driver */
static struct usb_device_id ddk_table[] =
{
	{
		USB_DEVICE(DDK_VENDOR_ID, DDK_PRODUCT_ID)
	},
	{} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, ddk_table);

static struct usb_driver ddk_driver =
{
	.name = "ddk_io",
	.probe = ddk_probe,
	.disconnect = ddk_disconnect,
	.id_table = ddk_table,
};

static int __init ddk_init(void)
{
	int result;

	/* Register this driver with the USB subsystem */
	if ((result = usb_register(&ddk_driver)))
	{
		printk(KERN_ERR "usb_register failed. Error number %d\n", result);
	}
	printk(KERN_INFO "DDK usb_registered\n");
	return result;
}

static void __exit ddk_exit(void)
{
	/* Deregister this driver with the USB subsystem */
	usb_deregister(&ddk_driver);
	printk(KERN_INFO "DDK usb_deregistered\n");
}

module_init(ddk_init);
module_exit(ddk_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia <email@sarika-pugs.com>");
MODULE_DESCRIPTION("USB LED Device Driver for DDK v1.1");
