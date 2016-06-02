#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_IN 0x81
#define BULK_EP_OUT 0x02
#define MAX_BUFF_SIZE 512
#define TIMEOUT 5000 

// Private structure used to send data to the USB Subsystem
struct private_data
{
	int endPtAddr[4];
	int buffSize[4];
	unsigned char *buffer[4];
	struct usb_device *dev;
	struct usb_class_driver usbClsDrv;
};

static struct usb_device *device;
static struct usb_class_driver usbClass;
static unsigned char usbDataBuff [MAX_BUFF_SIZE];


static int pen_open (struct inode *i, struct file *f)
{
	printk (KERN_INFO "Pen drive open");
	return 0;
}


static int pen_close (struct inode *i, struct file *f)
{
	printk (KERN_INFO "Pen drive close");
	return 0;
}

static ssize_t pen_read (struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
	int retVal = 0;
	int readCnt = 0;
	if ((retVal = usb_bulk_msg (device, usb_rcvbulkpipe(device, BULK_EP_IN), usbDataBuff, MAX_BUFF_SIZE, &readCnt, TIMEOUT)) < 0)
	{
		printk (KERN_ERR "Error while reading data from PEN DRV, %d", retVal);
	}

	if (copy_to_user (buf, usbDataBuff, MIN (cnt, readCnt)))
	{
		return -EFAULT;
	}

	return MIN (cnt, readCnt);
}

static ssize_t pen_write (struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{
	int retval;
	int wrote_cnt = MIN (cnt, MAX_BUFF_SIZE);
	
	if (copy_from_user (usbDataBuff, buf, MIN (cnt, MAX_BUFF_SIZE)))
	{
		return -EFAULT;
	}
	
	/* Write the data into the bulk endpoint */
	if ((retval = usb_bulk_msg (device, usb_sndbulkpipe(device, BULK_EP_OUT), usbDataBuff, MIN(cnt, MAX_BUFF_SIZE), &wrote_cnt, TIMEOUT)) < 0)
	{	
		printk(KERN_ERR "Bulk message returned %d\n", retval);
		return retval;
	}
	
	return wrote_cnt;
}

static struct file_operations fops = 
{
	.open = pen_open,
	.release = pen_close,
	.write = pen_write,
	.read = pen_read,
};

// USB Subsystem will call probe function whenever any new usb device found
static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	int retVal = 0;
	int iCnt = 0;
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	struct private_data *devData;

	iface_desc = interface->cur_altsetting;

	// Loop to get info of all available endpoints
	for (iCnt = 0; iCnt < iface_desc->desc.bNumEndpoints; iCnt++)
	{
		// Store endpoint
		endpoint = &iface_desc->endpoint[iCnt].desc;

		// Check whether endpoint is in or not and 
		// whether transfer type in bulk or not
		if (!devData->endPtAddr &&
			((endpoint->bEndpointAddress & USB_DIR_IN)) &&
			((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK))
		{
			// Fill device data
			devData->buffSize[iCnt] = endpoint->wMaxPacketSize;
			devData->endPtAddr[iCnt] = endpoint->bEndpointAddress;
        	devData->buffer[iCnt] = kmalloc(devData->buffSize[iCnt], GFP_KERNEL);
			if (!devData->buffer[iCnt])
			{
				printk (KERN_ERR "Failed to allocate device buffer");
			}
		}
	}

	/* Get pointer of usb_device */
	devData->dev = interface_to_usbdev(interface);

	// Create entry in /dev/.
	devData->usbClsDrv.name = "usb/pen%d";
	devData->usbClsDrv.fops = &fops;

	// Register device
	if ((retVal = usb_register_dev (interface, &devData->usbClsDrv)) < 0)
	{
		printk (KERN_ERR "Failed to register usb device with error %d", retVal);
	}
	else
	{
		printk (KERN_INFO "Register USB device with minor %d", interface->minor);
	}

	// Provide interface data to the USB Subsystem
	usb_set_intfdata (interface, &devData);

    return retVal;
}
 
static void pen_disconnect(struct usb_interface *interface)
{
	usb_deregister_dev (interface, &usbClass);
    printk(KERN_INFO "Pen drive removed\n");
}
 
static struct usb_device_id pen_table[] =
{
    { USB_DEVICE(0x0781, 0x5577) },
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, pen_table);//Allow user-space tools to figure out what devices this driver can control
 
static struct usb_driver pen_driver =
{
	.owner = THIS_MODULE,
    .name = "pen_driver",
    .id_table = pen_table,
    .probe = pen_probe,
    .disconnect = pen_disconnect,
};
 
static int __init pen_init(void)
{
    return usb_register(&pen_driver);
}
 
static void __exit pen_exit(void)
{
    usb_deregister(&pen_driver);
}
 
module_init(pen_init);
module_exit(pen_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pratik Panchal");
MODULE_DESCRIPTION("USB Pendrive Registration Driver");
