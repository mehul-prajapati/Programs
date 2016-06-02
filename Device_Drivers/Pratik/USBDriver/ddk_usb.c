#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#ifndef ENABLE_USB_DEV
#include <linux/cdev.h>
#endif

#include "ddk_usb.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

static struct ddk_device g_ddk_dev;
static DEFINE_MUTEX(m);

#ifdef ENABLE_FILE_OPS
#ifndef ENABLE_USB_DEV
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
#endif

static int ddk_open(struct inode *i, struct file *f)
{
	if (!mutex_trylock(&m))
	{
		return -EBUSY;
	}
	g_ddk_dev.buf_left_over[MEM_IN] = 0;
	g_ddk_dev.buf_left_over[MEM_OUT] = 0;
	g_ddk_dev.buf_left_over[SER_IN] = 0;
	g_ddk_dev.buf_left_over[SER_OUT] = 0;
	f->private_data = (void *)&g_ddk_dev;
	return 0;
}
static int ddk_close(struct inode *i, struct file *f)
{
	mutex_unlock(&m);
	return 0;
}
static ssize_t ddk_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
	struct ddk_device *dev = (struct ddk_device *)(f->private_data);
	int read_size, read_cnt;
	int retval;
	int i;

	if ((dev->attrib[SER_IN] != USB_ENDPOINT_XFER_INT) || !dev->ep[SER_IN])
	{
		return -EINVAL;
	}
	printk(KERN_INFO "Read request for %d bytes\n", cnt);
	/* Check for left over data */
	if (dev->buf_left_over[SER_IN])
	{
		read_size = dev->buf_left_over[SER_IN];
		dev->buf_left_over[SER_IN] = 0;
	}
	else
	{
		/* Read the data in the int endpoint */
		/* Using buf may cause sync issues */
		retval = usb_interrupt_msg(dev->device, usb_rcvintpipe(dev->device, dev->ep[SER_IN]),
			dev->buf[SER_IN], dev->buf_size[SER_IN], &read_size, 5000);
		if (retval)
		{
			printk(KERN_ERR "Interrupt message returned %d\n", retval);
			return retval;
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
	if (copy_to_user(buf, dev->buf[SER_IN], read_cnt))
	{
		dev->buf_left_over[SER_IN] = read_size;
		return -EFAULT;
	}
	for (i = cnt; i < read_size; i++)
	{
		dev->buf[SER_IN][i - cnt] = dev->buf[SER_IN][i];
	}
	if (cnt < read_size)
	{
		dev->buf_left_over[SER_IN] = read_size - cnt;
	}
	else
	{
		dev->buf_left_over[SER_IN] = 0;
	}
	printk(KERN_INFO "Actually read %d bytes (Sent to user: %d)\n", read_size, read_cnt);

	return read_cnt;
}
static ssize_t ddk_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{
	struct ddk_device *dev = (struct ddk_device *)(f->private_data);
	int write_size, wrote_size, wrote_cnt;
	int retval;

	if ((dev->attrib[SER_OUT] != USB_ENDPOINT_XFER_INT) || !dev->ep[SER_OUT])
	{
		return -EINVAL;
	}
	wrote_cnt = 0;
	while (wrote_cnt < cnt)
	{
		write_size = MIN(dev->buf_size[SER_OUT], cnt - wrote_cnt /* Remaining */);
		/* Using buf may cause sync issues */
		if (copy_from_user(dev->buf[SER_OUT], buf + wrote_cnt, write_size))
		{
			return -EFAULT;
		}
		/* Send the data out the int endpoint */
		retval = usb_interrupt_msg(dev->device, usb_sndintpipe(dev->device, dev->ep[SER_OUT]),
			dev->buf[SER_OUT], write_size, &wrote_size, 0);
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
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int ddk_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long ddk_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
	struct ddk_device *dev = (struct ddk_device *)(f->private_data);
	char buf[4];
	int val;
	IoctlData ival;
	int read_size, wrote_size;
	int retval;

	switch (cmd)
	{
		case DDK_LED_GET:
			/* Control IN */
			retval = usb_control_msg(dev->device, usb_rcvctrlpipe(dev->device, 0),
						CUSTOM_RQ_GET_LED_STATUS,
						USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
						0, 0, buf, sizeof(buf), 0);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			val = buf[0];
			if (copy_to_user((int *)arg, &val, sizeof(int)))
			{
				return -EFAULT;
			}
			break;
		case DDK_LED_SET:
			/* Control OUT */
			val = arg;
			retval = usb_control_msg(dev->device, usb_sndctrlpipe(dev->device, 0),
						CUSTOM_RQ_SET_LED_STATUS,
						USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
						val, 0, buf, sizeof(buf), 0);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			break;
		case DDK_RD_OFF_GET:
			/* Control IN */
			retval = usb_control_msg(dev->device, usb_rcvctrlpipe(dev->device, 0),
						CUSTOM_RQ_GET_MEM_RD_OFFSET,
						USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
						0, 0, buf, sizeof(buf), 0);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			val = *(short *)buf;
			if (copy_to_user((int *)arg, &val, sizeof(int)))
			{
				return -EFAULT;
			}
			break;
		case DDK_RD_OFF_SET:
			/* Control OUT */
			val = arg;
			retval = usb_control_msg(dev->device, usb_sndctrlpipe(dev->device, 0),
						CUSTOM_RQ_SET_MEM_RD_OFFSET,
						USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
						val, 0, buf, sizeof(buf), 0);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			break;
		case DDK_WR_OFF_GET:
			/* Control IN */
			retval = usb_control_msg(dev->device, usb_rcvctrlpipe(dev->device, 0),
						CUSTOM_RQ_GET_MEM_WR_OFFSET,
						USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
						0, 0, buf, sizeof(buf), 0);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			val = *(short *)buf;
			if (copy_to_user((int *)arg, &val, sizeof(int)))
			{
				return -EFAULT;
			}
			break;
		case DDK_WR_OFF_SET:
			/* Control OUT */
			val = arg;
			retval = usb_control_msg(dev->device, usb_sndctrlpipe(dev->device, 0),
						CUSTOM_RQ_SET_MEM_WR_OFFSET,
						USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
						val, 0, buf, sizeof(buf), 0);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			break;
		case DDK_MEM_SIZE_GET:
			/* Control IN */
			retval = usb_control_msg(dev->device, usb_rcvctrlpipe(dev->device, 0),
						CUSTOM_RQ_GET_MEM_SIZE,
						USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
						0, 0, buf, sizeof(buf), 0);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			val = *(short *)buf;
			if (copy_to_user((int *)arg, &val, sizeof(int)))
			{
				return -EFAULT;
			}
			break;
		case DDK_DATA_GET:
			/* Interrupt IN */
			if ((dev->attrib[MEM_IN] != USB_ENDPOINT_XFER_INT) || !dev->ep[MEM_IN])
			{
				return -EINVAL;
			}
			retval = usb_interrupt_msg(dev->device,
						usb_rcvintpipe(dev->device, dev->ep[MEM_IN]),
						dev->buf[MEM_IN], dev->buf_size[MEM_IN], &read_size, 0);
			if (retval)
			{
				printk(KERN_ERR "Interrupt message returned %d\n", retval);
				return retval;
			}
			ival.len = read_size;
			memcpy(&ival.data, dev->buf[MEM_IN], ival.len);
			if (copy_to_user((IoctlData *)arg, &ival, sizeof(IoctlData)))
			{
				return -EFAULT;
			}
			break;
		case DDK_DATA_SET:
			/* Interrupt OUT */
			if ((dev->attrib[MEM_OUT] != USB_ENDPOINT_XFER_INT) || !dev->ep[MEM_OUT])
			{
				return -EINVAL;
			}
			if (copy_from_user(&ival, (IoctlData *)arg, sizeof(IoctlData)))
			{
				return -EFAULT;
			}
			retval = usb_interrupt_msg(dev->device,
						usb_sndintpipe(dev->device, dev->ep[MEM_OUT]),
						ival.data, ival.len, &wrote_size, 0);
			if (retval)
			{
				printk(KERN_ERR "Interrupt message returned %d\n", retval);
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
	.read = ddk_read,
	.write = ddk_write,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
	.ioctl = ddk_ioctl,
#else
	.unlocked_ioctl = ddk_ioctl,
#endif
};

#ifndef ENABLE_USB_DEV
static int char_register_dev(struct usb_interface *interface, struct usb_class_driver *class)
{
	int ret;
	struct device *dev_ret;

	if ((ret = alloc_chrdev_region(&dev, 0, 1, "ddk_usb")) < 0)
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
#endif

static int get_ep_index(int ep)
{
	switch (ep)
	{
		case MEM_EP_IN:
			return MEM_IN;
			break;
		case MEM_EP_OUT:
			return MEM_OUT;
			break;
		case SER_EP_IN:
			return SER_IN;
			break;
		case SER_EP_OUT:
			return SER_OUT;
			break;
		default:
			return -1;
			break;
	}
}

static int ddk_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	int i, ep_i;
	int retval;

	iface_desc = interface->cur_altsetting;
	printk(KERN_INFO "DDK USB i/f %d now probed: (%04X:%04X)\n",
		iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
	printk(KERN_INFO "ID->bNumEndpoints: %02X\n", iface_desc->desc.bNumEndpoints);
	printk(KERN_INFO "ID->bInterfaceClass: %02X\n", iface_desc->desc.bInterfaceClass);

	/* Set up the endpoint information. Assuming there is 2 ins & 2 outs */
	for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
	{
		endpoint = &iface_desc->endpoint[i].desc;

		printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n", i, endpoint->bEndpointAddress);
		printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n", i, endpoint->bmAttributes);
		printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n", i, endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);

		ep_i = get_ep_index(endpoint->bEndpointAddress);
		if (ep_i == -1)
		{
			printk(KERN_ERR "Unexpected endpoint number found.\n");
			retval = -ENOENT;
			goto probe_err;
		}
		printk(KERN_INFO "EP Index: %d\n", ep_i);
		g_ddk_dev.ep[ep_i] = endpoint->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
		g_ddk_dev.attrib[ep_i] = endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK;
		g_ddk_dev.buf_size[ep_i] = endpoint->wMaxPacketSize;
		g_ddk_dev.buf[ep_i] = kmalloc(endpoint->wMaxPacketSize, GFP_KERNEL);
		if (!g_ddk_dev.buf[ep_i])
		{
			printk(KERN_ERR "Not able to get memory for in ep buffer of this device.\n");
			retval = -ENOMEM;
			goto probe_err;
		}
		g_ddk_dev.buf_left_over[ep_i] = 0;
	}

	g_ddk_dev.device = interface_to_usbdev(interface);

#ifdef ENABLE_FILE_OPS
	g_ddk_dev.class.name = "usb/ddk%d";
	g_ddk_dev.class.fops = &fops;
#ifndef ENABLE_USB_DEV
	retval = char_register_dev(interface, &g_ddk_dev.class);
#else
	retval = usb_register_dev(interface, &g_ddk_dev.class);
#endif
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
	if (g_ddk_dev.buf[SER_OUT])
	{
		kfree(g_ddk_dev.buf[SER_OUT]);
	}
	if (g_ddk_dev.buf[SER_IN])
	{
		kfree(g_ddk_dev.buf[SER_IN]);
	}
	if (g_ddk_dev.buf[MEM_OUT])
	{
		kfree(g_ddk_dev.buf[MEM_OUT]);
	}
	if (g_ddk_dev.buf[MEM_IN])
	{
		kfree(g_ddk_dev.buf[MEM_IN]);
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
#ifndef ENABLE_USB_DEV
	char_deregister_dev(interface, &ddk_dev->class);
#else
	usb_deregister_dev(interface, &ddk_dev->class);
#endif
#endif

	mutex_lock(&m);
	kfree(ddk_dev->buf[SER_OUT]);
	kfree(ddk_dev->buf[SER_IN]);
	kfree(ddk_dev->buf[MEM_OUT]);
	kfree(ddk_dev->buf[MEM_IN]);
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
	.name = "ddk",
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
MODULE_DESCRIPTION("USB Device Driver for DDK v1.1");
