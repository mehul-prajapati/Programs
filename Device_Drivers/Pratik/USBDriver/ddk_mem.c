#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#ifdef ENABLE_FILE_OPS
#include <asm/atomic.h>
#endif

#include "ddk_mem.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

enum
{
	e_read,
	e_write
};

static struct ddk_device g_ddk_dev;
DEFINE_MUTEX(m);

#ifdef ENABLE_FILE_OPS
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
static atomic_t open_cnt;

static int get_size(struct ddk_device *dev)
{
	int retval;
	short val;

	/* Control IN */
	retval = usb_control_msg(dev->device, usb_rcvctrlpipe(dev->device, 0),
			CUSTOM_RQ_GET_MEM_SIZE, USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			0, 0, &val, sizeof(val), 0);
	if (retval < 0)
	{
		return retval;
	}
	else
	{
		return val;
	}
}
static int get_off(struct ddk_device *dev, int dir)
{
	int retval;
	short val;

	/* Control IN */
	retval = usb_control_msg(dev->device, usb_rcvctrlpipe(dev->device, 0),
			(dir == e_read) ? CUSTOM_RQ_GET_MEM_RD_OFFSET : CUSTOM_RQ_GET_MEM_WR_OFFSET,
			USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			0, 0, &val, sizeof(val), 0);
	if (retval < 0)
	{
		return retval;
	}
	else
	{
		return val;
	}
}
static int set_off(struct ddk_device *dev, int dir, int offset)
{
	/* Control OUT */
	return usb_control_msg(dev->device, usb_sndctrlpipe(dev->device, 0),
			(dir == e_read) ? CUSTOM_RQ_SET_MEM_RD_OFFSET : CUSTOM_RQ_SET_MEM_WR_OFFSET,
			USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
			offset, 0, NULL, 0, 0);
}

static int ddk_open(struct inode *i, struct file *f)
{
	struct ddk_device *dev = &g_ddk_dev;
	struct ddk_device_file *ddf;
	int retval;

	mutex_lock(&m);
	if (dev->in_buf_size == 0)
	{
		mutex_unlock(&m);
		return -ENODEV;
	}
	ddf = kmalloc(sizeof(struct ddk_device_file) +
					dev->in_buf_size + dev->out_buf_size, GFP_KERNEL);
	if (ddf == NULL)
	{
		return -ENOMEM;
	}
	ddf->in_buf = (unsigned char *)(ddf) + sizeof(struct ddk_device_file);
	ddf->out_buf = ddf->in_buf + dev->in_buf_size;
	ddf->in_buf_left_over = 0;
	f->private_data = (void *)ddf;
	mutex_unlock(&m);

	if ((retval = set_off(dev, e_read, 0)) < 0)
	{
		return retval;
	}
	if ((retval = set_off(dev, e_write, 0)) < 0)
	{
		return retval;
	}

	printk(KERN_INFO "Already open: %d\n", atomic_read(&open_cnt));
	atomic_inc(&open_cnt);
	return 0;
}
static int ddk_close(struct inode *i, struct file *f)
{
	atomic_dec(&open_cnt);
	kfree(f->private_data);
	return 0;
}
static ssize_t ddk_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{
	struct ddk_device *dev = &g_ddk_dev;
	struct ddk_device_file *ddf = (struct ddk_device_file *)(f->private_data);
	int write_size, wrote_size, wrote_cnt;
	int retval;

	/* Multiple writes may cause sync issues - need mutex protection over write */
	if ((retval = set_off(dev, e_write, (int)(*off))) < 0)
	{
		return retval;
	}
	wrote_cnt = 0;
	while (wrote_cnt < cnt)
	{
		write_size = MIN(dev->out_buf_size, cnt - wrote_cnt /* Remaining */);
		/* Using buf may cause sync issues */
		if (copy_from_user(ddf->out_buf, buf + wrote_cnt, write_size))
		{
			return -EFAULT;
		}
		/* Send the data out the int endpoint */
		retval = usb_interrupt_msg(dev->device, usb_sndintpipe(dev->device, dev->out_ep),
			ddf->out_buf, write_size, &wrote_size, 0);
		if (retval)
		{
			printk(KERN_ERR "Interrupt message returned %d\n", retval);
			return retval;
		}
		printk(KERN_INFO "Wrote %d bytes\n", wrote_size);
		wrote_cnt += wrote_size;
	}
	*off += wrote_cnt;
	/*
	 * TODO: In the msleep below, 60 also works, but < 55 doesn't - causes a
	 * protocol error in the set_off below. Question is why? Any issues with fw
	 * or the poll interval has to be considered for the above interrupt msg'ing
	 */
	msleep(100);
	/* Syncing the read & write offsets on the device. write offset is already updated */
	if ((retval = set_off(dev, e_read, (int)(*off))) < 0)
	{
		return retval;
	}

	return wrote_cnt;
}
static ssize_t ddk_read(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
	struct ddk_device *dev = &g_ddk_dev;
	struct ddk_device_file *ddf = (struct ddk_device_file *)(f->private_data);
	int read_size, read_cnt;
	int retval;
	int i;

	printk(KERN_INFO "Read request for %d bytes\n", cnt);
	/* Check for left over data */
	if (ddf->in_buf_left_over)
	{
		read_size = ddf->in_buf_left_over;
		ddf->in_buf_left_over = 0;
	}
	else
	{
		/* Multiple reads may cause sync issues - need mutex protection over read */
		if ((retval = set_off(dev, e_read, (int)(*off))) < 0)
		{
			return retval;
		}
		/* Read the data in the int endpoint */
		/* Using buf may cause sync issues - need mutex protection */
		retval = usb_interrupt_msg(dev->device, usb_rcvintpipe(dev->device, dev->in_ep),
			ddf->in_buf, dev->in_buf_size, &read_size, 0);
		printk(KERN_INFO "Off: %Ld; Ret: %d\n", *off, retval);
		if (retval)
		{
			printk(KERN_ERR "Interrupt message returned %d\n", retval);
			return retval;
		}
		/* Syncing the read & write offsets on the device. read offset is already updated */
		if ((retval = set_off(dev, e_write, (int)(*off + read_size))) < 0)
		{
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
	if (copy_to_user(buf, ddf->in_buf, read_cnt))
	{
		ddf->in_buf_left_over = read_size;
		return -EFAULT;
	}
	for (i = cnt; i < read_size; i++)
	{
		ddf->in_buf[i - cnt] = ddf->in_buf[i];
	}
	if (cnt < read_size)
	{
		ddf->in_buf_left_over = read_size - cnt;
	}
	else
	{
		ddf->in_buf_left_over = 0;
	}
	printk(KERN_INFO "Actually read %d bytes (Sent to user: %d)\n", read_size, read_cnt);
	*off += read_cnt;

	return read_cnt;
}
static loff_t ddk_llseek(struct file *f, loff_t offset, int whence)
{
	struct ddk_device *dev = &g_ddk_dev;
	int cur, size = get_size(dev);
	int retval;

	if (size < 0)
	{
		return size; // Actually error
	}
	switch (whence)
	{
		case SEEK_SET:
			break;
		case SEEK_CUR:
			if ((cur = get_off(dev, e_read)) < 0)
			{
				return cur;
			}
			offset += cur;
			break;
		case SEEK_END:
			offset += size;
			break;
		default:
			return -EINVAL;
			break;
	}
	if (offset < 0)
	{
		offset = 0;
	}
	else if (offset > size)
	{
		offset = size;
	}
	if ((retval = set_off(dev, e_read, (int)(offset))) < 0)
	{
		return retval;
	}
	if ((retval = set_off(dev, e_write, (int)(offset))) < 0)
	{
		return retval;
	}
	f->f_pos = offset;
	return offset;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int ddk_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long ddk_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
	struct ddk_device *dev = &g_ddk_dev;
	int retval;

	switch (cmd)
	{
		case DDK_RD_OFF_GET:
			retval = get_off(dev, e_read);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			if (copy_to_user((int *)arg, &retval, sizeof(int)))
			{
				return -EFAULT;
			}
			break;
		case DDK_RD_OFF_SET:
			retval = set_off(dev, e_read, (int)(arg));
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			break;
		case DDK_WR_OFF_GET:
			retval = get_off(dev, e_write);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			if (copy_to_user((int *)arg, &retval, sizeof(int)))
			{
				return -EFAULT;
			}
			break;
		case DDK_WR_OFF_SET:
			retval = set_off(dev, e_write, (int)(arg));
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			break;
		case DDK_MEM_SIZE_GET:
			retval = get_size(dev);
			if (retval < 0)
			{
				printk(KERN_ERR "Control message returned %d\n", retval);
				return retval;
			}
			if (copy_to_user((int *)arg, &retval, sizeof(int)))
			{
				return -EFAULT;
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
	.write = ddk_write,
	.read = ddk_read,
	.llseek = ddk_llseek,
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

	if ((ret = alloc_chrdev_region(&dev, 0, 1, "ddk_mem")) < 0)
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
	struct ddk_device ddk_dev;
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	int i;
	int retval;

	iface_desc = interface->cur_altsetting;
	/* Set up the endpoint information related to serial */
	memset(&ddk_dev, 0, sizeof(ddk_dev));
	for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
	{
		endpoint = &iface_desc->endpoint[i].desc;

		if (endpoint->bEndpointAddress == MEM_EP_IN)
		{
			ddk_dev.in_ep = endpoint->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
			ddk_dev.in_buf_size = endpoint->wMaxPacketSize;
		}
		if (endpoint->bEndpointAddress == MEM_EP_OUT)
		{
			ddk_dev.out_ep = endpoint->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;
			ddk_dev.out_buf_size = endpoint->wMaxPacketSize;
		}
	}

	ddk_dev.device = interface_to_usbdev(interface);

	/* Prevent ddk_open() from racing on g_ddk_dev in ddk_probe() */
	mutex_lock(&m);
#ifdef ENABLE_FILE_OPS
	ddk_dev.class.name = "usb/ddk_mem%d";
	ddk_dev.class.fops = &fops;
	retval = char_register_dev(interface, &ddk_dev.class);
	if (retval)
	{
		/* Something prevented us from registering this driver */
		mutex_unlock(&m);
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

	memcpy(&g_ddk_dev, &ddk_dev, sizeof(struct ddk_device));
	usb_set_intfdata(interface, &g_ddk_dev);
	mutex_unlock(&m);

	return 0;
}

static void ddk_disconnect(struct usb_interface *interface)
{
	struct ddk_device *ddk_dev;

	printk(KERN_INFO "Releasing Minor: %d\n", interface->minor);

	ddk_dev = (struct ddk_device *)(usb_get_intfdata(interface));
	usb_set_intfdata(interface, NULL);

	/* Prevent ddk_open() from racing on g_ddk_dev in ddk_disconnect() */
	mutex_lock(&m);
#ifdef ENABLE_FILE_OPS
	/* Give back our minor */
	char_deregister_dev(interface, &ddk_dev->class);
#endif

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
	.name = "ddk_mem",
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
MODULE_DESCRIPTION("USB Memory Device Driver for DDK v1.1");
