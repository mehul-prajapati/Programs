//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
#define DEV_CNT			1

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
static int my_open(struct inode *inode, struct file *filep);
static int my_close(struct inode *inode, struct file *filep);
static int my_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static int my_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
static int major;
static int minor;
static struct cdev c_dev;
dev_t dev;
static char kernBuf[20];
static struct file_operations fops = {

        .owner = THIS_MODULE,
        .open = my_open,
	.read = my_read,
	.write = my_write,
        .release = my_close

};

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
/* S_IRUGO - permission value ( read only ) */
module_param(major, int, S_IRUGO);

static int __init my_drv_init(void)
{
	int result = 0;

	printk(KERN_ALERT "Registering the module\n"); /* The string KERN_ALERT is the priority of the message.*/
	
	if (major) 
	{
		dev = MKDEV(major, minor); 
		result = register_chrdev_region(dev, DEV_CNT, "myNode");
	}
	else
	{
		result = alloc_chrdev_region(&dev, minor, DEV_CNT, "myNode");
	}

	if (result < 0) 
	{
		printk(KERN_WARNING "Cant get major nr %d\n", major);
		
		return result;
	}
	else 
	{
		major = MAJOR(dev);
		minor = MINOR(dev);
		printk(KERN_ALERT "Got major nr %d and minor nr %d\n", major, minor);
	}

    	cdev_init(&c_dev, &fops);
 
    	if ((result = cdev_add(&c_dev, dev, DEV_CNT)) < 0)
    	{
		printk(KERN_WARNING "char dev init failed\n");
        	return result;
    	}

	return 0;
}

static void __exit my_drv_exit(void)
{
	printk(KERN_ALERT "Exit module\n");

	cdev_del(&c_dev);	
	unregister_chrdev_region(dev, DEV_CNT);
}

static int my_open(struct inode *inode, struct file *filep)
{
	printk(KERN_ALERT "Opening the device\n");
	memset(kernBuf, 0x00, sizeof(kernBuf));

	return 0;
}

static int my_close(struct inode *inode, struct file *filep)
{
        printk(KERN_ALERT "Closing the device\n");

	return 0;
}

static int my_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	long int bytes_read = 0;

	bytes_read = copy_to_user(buf, kernBuf, 20);

	if (bytes_read)
	{
		printk(KERN_ALERT "copy to user failed\n");

		return -EFAULT;
	}

	//printk(KERN_ALERT "Bytes read = %ld\n", bytes_read);

	return 0;	
}

static int my_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
        if (copy_from_user(kernBuf, buf, 20))
        {
                printk(KERN_ALERT "copy from user failed\n");

                return -EFAULT;
        }

        return 0;
}

module_init(my_drv_init); /* module is loaded into the kernel */
module_exit(my_drv_exit); /* one for when the module is removed */
//==================================================================
//================= @KERNEL MACROS =================================
//==================================================================
MODULE_LICENSE("Dual BSD/GPL"); /* It is used to tell the kernel that this module bears a free license; without such a declaration, the kernel
                                   complains when the module is loaded. */
MODULE_AUTHOR("Mehul");
MODULE_DESCRIPTION("My Driver");
