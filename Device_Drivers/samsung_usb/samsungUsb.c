/*
 * Author : Mehul Prajapati <Software Developer>
 *
 *
 *
 */
//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/usb.h>
#include <linux/kernel.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define GALAXY_Y_VENDOR_ID		0x04E8
#define GALAXY_Y_PRODUCT_ID		0x685E
#define SAMSUNG_MINOR			17
#define MIN(a,b) 			(((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 			0x01
#define BULK_EP_IN 			0x81
#define MAX_PKT_SIZE 			512
 
//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
static int samsung_usb_probe(struct usb_interface *interface, const struct usb_device_id *id);
static void samsung_usb_disconnect(struct usb_interface *interface);
static int usb_open(struct inode *i, struct file *f);
static int usb_close(struct inode *i, struct file *f);
static int usb_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
static int usb_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
static struct usb_device_id my_usb_id[] = {

	{ USB_DEVICE(GALAXY_Y_VENDOR_ID, GALAXY_Y_PRODUCT_ID) },
	
	{ }
};

MODULE_DEVICE_TABLE(usb, my_usb_id);

static struct usb_driver my_usb_driver = {

	.name = "Samsung Galaxy Y Duos",
	.probe= samsung_usb_probe,
	.disconnect = samsung_usb_disconnect,
	.id_table = my_usb_id

};

typedef struct {

	struct usb_device *pUsbDev;
	struct usb_interface *pInterface;
	struct urb *pCtrlurb;
	
	struct usb_ctrlrequest ctrlReq;
	
	unsigned char *pBlk_in_buf;
	size_t blk_in_len;
	
	__u8 blk_in_addr;
	__u8 blk_out_addr;

} samsung_usb;

static struct usb_device *device;
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];

static struct file_operations usb_fops = {

	.owner = THIS_MODULE,
	.open = usb_open,
	.write = usb_write,
	.release = usb_close,
	.read = usb_read
};

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
static int usb_open(struct inode *i, struct file *f)
{
	printk(KERN_ALERT "Opening USB Device\n");

	return 0;
}

static int usb_close(struct inode *i, struct file *f)
{
	printk(KERN_ALERT "Closing USB Device\n");

	return 0;
}

static int usb_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int read_cnt;
    int retval = 0;
 
    /* Read the data from the bulk endpoint */
    retval = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_EP_IN), bulk_buf, MAX_PKT_SIZE, &read_cnt, 5000);
    
    if (retval)
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        
	return retval;
    }

    if (copy_to_user(buf, bulk_buf, MIN(count, read_cnt)))
    {
        return -EFAULT;
    }
 
    return MIN(count, read_cnt);
}

static int usb_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    int retval;
    int wrote_cnt = MIN(count, MAX_PKT_SIZE);
 
    if (copy_from_user(bulk_buf, buf, MIN(count, MAX_PKT_SIZE)))
    {
        return -EFAULT;
    }
 
    /* Write the data into the bulk endpoint */
    retval = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_EP_OUT), bulk_buf, MIN(count, MAX_PKT_SIZE), &wrote_cnt, 5000);
    
    if (retval)
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }
 
    return wrote_cnt;
}

static int __init samsung_usb_init(void)
{
	int retVal = 0;

	/* Register with USB core */
	retVal = usb_register(&my_usb_driver);

	if (retVal < 0)
	{
		printk(KERN_ALERT "Device registration failed\n");
	}
	else
	{
		printk(KERN_ALERT "Device registration success ;) \n");
	}

	return retVal;
}

static void __exit samsung_usb_exit(void)
{
	/* De register with USB core */
	usb_deregister(&my_usb_driver);
}

static int samsung_usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    	struct usb_host_interface *iface_desc;
    	struct usb_endpoint_descriptor *endpoint;
   	int i, retval;
 
    	iface_desc = interface->cur_altsetting;
    	
	printk(KERN_INFO "Pen i/f %d now probed: (%04X:%04X)\n", iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
    	printk(KERN_INFO "ID->bNumEndpoints: %02X\n", iface_desc->desc.bNumEndpoints);
    	printk(KERN_INFO "ID->bInterfaceClass: %02X\n", iface_desc->desc.bInterfaceClass);
 
    	for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
    	{
        	endpoint = &iface_desc->endpoint[i].desc;
 
        	printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n", i, endpoint->bEndpointAddress);
        	printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n", i, endpoint->bmAttributes);
        	printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n", i, endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);
    	}

	/* get device desc */ 
    	device = interface_to_usbdev(interface);;

	class.name = "usb/pen%d";
    	class.fops = &usb_fops;
    
	if ((retval = usb_register_dev(interface, &class)) < 0)
    	{
        	/* Something prevented us from registering this driver */
        	printk(KERN_ALERT "Not able to get a minor for this device.");
    	}
    	else
    	{
        	printk(KERN_INFO "Minor obtained: %d\n", interface->minor);
    	}
 
    	return retval;
}

static void samsung_usb_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "Pen i/f %d now disconnected\n", interface->cur_altsetting->desc.bInterfaceNumber);

	usb_deregister_dev(interface, &class);
}

//==================================================================
//================= @KERNEL MACROS =================================
//==================================================================
MODULE_LICENSE("Dual BSD/GPL"); /* It is used to tell the kernel that this module bears a free license; without such a declaration, the kernel
                                   complains when the module is loaded. */
MODULE_AUTHOR("Mehul");
MODULE_DESCRIPTION("Samsung USB Driver");

/* Pre requisite */
module_init(samsung_usb_init);
module_exit(samsung_usb_exit);
