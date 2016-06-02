#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <asm/uaccess.h>

#include "ddk_u2s.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static struct ddk_device g_ddk_dev;
static DEFINE_MUTEX(m);

#ifdef ENABLE_FILE_OPS
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

static int ddk_open(struct inode *i, struct file *f)
{
	if (!mutex_trylock(&m))
	{
		return -EBUSY;
	}
	g_ddk_dev.in_buf_left_over = 0;
	f->private_data = (void *)&g_ddk_dev;
	return 0;
}
static int ddk_close(struct inode *i, struct file *f)
{
	mutex_unlock(&m);
	return 0;
}
static ssize_t ddk_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{
	struct ddk_device *dev;
	int write_size, wrote_size, wrote_cnt;
	int retval;

	dev = (struct ddk_device *)(f->private_data);
	wrote_cnt = 0;
	while (wrote_cnt < cnt)
	{
		write_size = MIN(dev->out_buf_size, cnt - wrote_cnt /* Remaining */);
		/* Using buf may cause sync issues */
		if (copy_from_user(dev->out_buf, buf + wrote_cnt, write_size))
		{
			return -EFAULT;
		}
		/* Send the data out the int endpoint */
		retval = usb_interrupt_msg(dev->device, usb_sndintpipe(dev->device, dev->out_ep),
			dev->out_buf, write_size, &wrote_size, 0);
		if (retval)
		{
			printk(KERN_ERR "Interrupt message returned %d\n", retval);
			return retval;
		}
		wrote_cnt += wrote_size;
		printk(KERN_INFO "Wrote %d bytes\n", wrote_size);
	}

	return wrote_cnt;
}
static ssize_t ddk_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
	struct ddk_device *dev;
	int read_size, read_cnt;
	int retval;
	int i;

	dev = (struct ddk_device *)(f->private_data);
	printk(KERN_INFO "Read request for %d bytes\n", cnt);
	/* Check for left over data */
	if (dev->in_buf_left_over)
	{
		read_size = dev->in_buf_left_over;
		dev->in_buf_left_over = 0;
	}
	else
	{
		/* Read the data in the int endpoint */
		/* Using buf may cause sync issues - need mutex protection */
		retval = usb_interrupt_msg(dev->device, usb_rcvintpipe(dev->device, dev->in_ep),
			dev->in_buf, dev->in_buf_size, &read_size, 10);
		if (retval)
		{
			printk(KERN_ERR "Interrupt message returned %d\n", retval);
			if (retval != -ETIMEDOUT)
			{
				return retval;
			}
			else /* Not really an error but no data available */
			{
				read_size = 0;
			}
		}
	}
	if (read_size <= cnt)
	{
		read_cnt = read_size;
	}
	else
	{
		read_cnt = cnt;
	}
	if (copy_to_user(buf, dev->in_buf, read_cnt))
	{
		dev->in_buf_left_over = read_size;
		return -EFAULT;
	}
	for (i = cnt; i < read_size; i++)
	{
		dev->in_buf[i - cnt] = dev->in_buf[i];
	}
	if (cnt < read_size)
	{
		dev->in_buf_left_over = read_size - cnt;
	}
	else
	{
		dev->in_buf_left_over = 0;
	}
	printk(KERN_INFO "Actually read %d bytes (Sent to user: %d)\n", read_size, read_cnt);

	return read_cnt;
}

static struct file_operations fops =
{
	.open = ddk_open,
	.release = ddk_close,
	.write = ddk_write,
	.read = ddk_read,
};

static int char_register_dev(struct usb_interface *interface, struct usb_class_driver *class)
{
	int ret;
	struct device *dev_ret;

	if ((ret = alloc_chrdev_region(&dev, 0, 1, "ddk_u2s")) < 0)
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
	/* Set up the endpoint information related to serial */
	for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
	{
		endpoint = &iface_desc->endpoint[i].desc;

		if (endpoint->bEndpointAddress == SER_EP_IN)
		{
			g_ddk_dev.in_ep = endpoint->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
			g_ddk_dev.in_buf_size = endpoint->wMaxPacketSize;
			g_ddk_dev.in_buf = kmalloc(endpoint->wMaxPacketSize, GFP_KERNEL);
			if (!g_ddk_dev.in_buf)
			{
				printk(KERN_ERR "Not able to get memory for in ep buffer of this device.\n");
				retval = -ENOMEM;
				goto probe_err;
			}
			g_ddk_dev.in_buf_left_over = 0;
		}
		if (endpoint->bEndpointAddress == SER_EP_OUT)
		{
			g_ddk_dev.out_ep = endpoint->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
			g_ddk_dev.out_buf_size = endpoint->wMaxPacketSize;
			g_ddk_dev.out_buf = kmalloc(endpoint->wMaxPacketSize, GFP_KERNEL);
			if (!g_ddk_dev.out_buf)
			{
				printk(KERN_ERR "Not able to get memory for out ep buffer of this device.\n");
				retval = -ENOMEM;
				goto probe_err;
			}
		}
	}

	g_ddk_dev.device = interface_to_usbdev(interface);

#ifdef ENABLE_FILE_OPS
	g_ddk_dev.class.name = "usb/ddk_u2s%d";
	g_ddk_dev.class.fops = &fops;
	retval = char_register_dev(interface, &g_ddk_dev.class);
	if (retval)
	{
		/* Something prevented us from registering this driver */
		printk(KERN_ERR "Not able to get a minor for this device.\n");
		goto probe_err;
	}
	else
	{
		printk(KERN_INFO "Minor obtained: %d\n", interface->minor);
	}
#else
	interface->minor = iface_desc->desc.bInterfaceNumber;
#endif

	usb_set_intfdata(interface, &g_ddk_dev);

	return 0;

probe_err:
	if (g_ddk_dev.out_buf)
	{
		kfree(g_ddk_dev.out_buf);
	}
	if (g_ddk_dev.in_buf)
	{
		kfree(g_ddk_dev.in_buf);
	}
	memset(&g_ddk_dev, 0, sizeof(g_ddk_dev));

	return retval;
}

static void ddk_disconnect(struct usb_interface *interface)
{
	struct ddk_device *ddk_dev;

	printk(KERN_INFO "Releasing Minor: %d\n", interface->minor);

	ddk_dev = (struct ddk_device *)(usb_get_intfdata(interface));
	usb_set_intfdata(interface, NULL);

#ifdef ENABLE_FILE_OPS
	/* Give back our minor */
	char_deregister_dev(interface, &ddk_dev->class);
#endif

	/* Prevent ddk_open() from racing ddk_disconnect() */
	mutex_lock(&m);
	kfree(ddk_dev->out_buf);
	kfree(ddk_dev->in_buf);
	memset(ddk_dev, 0, sizeof(ddk_dev));
	mutex_unlock(&m);

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
	.name = "ddk_u2s",
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
MODULE_DESCRIPTION("USB 2 Serial Device Driver for DDK v1.1");
