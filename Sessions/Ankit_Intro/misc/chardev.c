#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>

#define GET_VAR _IOR('q',1,int)
#define SET_VAR _IOW('q',2,int)
#define CLR_VAR _IO('q',3)

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Character Device Driver");
MODULE_AUTHOR("Ankit");

static char  msg[100]     = {0};
static short readPos      =  0;
static int   MAJOR_NUMBER = 91;

static int device_open    ( struct inode * , struct file *);
static int device_release ( struct inode * , struct file *);

static ssize_t device_read  ( struct file * , char *,       size_t , loff_t *);
static ssize_t device_write ( struct file * , const char *, size_t , loff_t *);
static long device_ioctl (struct file *, unsigned int, unsigned long);

int ker_var = 10;
/**********************/
/* callback functions */
/**********************/

static struct file_operations file_opts = 
{
    .read = device_read,
    .open = device_open,
    .write = device_write,
    .release = device_release,
    .unlocked_ioctl = device_ioctl    
};


/************************************/
/* called when the module is loaded */
/************************************/

int init_module(void)
{
	int t;
	printk("\n CHARDEV_DEBUG file: %s, func: %s, line %d", __FILE__,__func__,__LINE__);    
	t = register_chrdev(MAJOR_NUMBER, "chardev", &file_opts);
	if(t<0) 
	{
		printk(KERN_ALERT "Device registration Failed !\n");
	}
	else
	{
		printk(KERN_ALERT "Device successfully registered !\n");
	}	

   return t;
}



/**************************************/
/* called when the module is unloaded */
/**************************************/

void cleanup_module(void)
{
	printk("CHARDEV_DEBUG file: %s, func: %s, line %d\n", __FILE__,__func__,__LINE__); 
	unregister_chrdev(91, "chardev");
}



/*************************************************************/
/* called when 'open' system call is done on the device file */
/*************************************************************/

static int device_open(struct inode *inodep, struct file *filep)
{
	printk("CHARDEV_DEBUG file: %s, func: %s, line %d\n", __FILE__,__func__,__LINE__); 
	printk(KERN_ALERT "Device opened successfully\n");
	return 0;
}


/*************************************************************/
/* called when 'read' system call is done on the device file */
/*************************************************************/

static ssize_t device_read(struct file *filep, char *buff, size_t len, loff_t *off)
{
	short count = 0;
	printk("CHARDEV_DEBUG file: %s, func: %s, line %d\n", __FILE__,__func__,__LINE__); 
	while(len && (msg[readPos]!=0))
	{
		put_user(msg[readPos],buff++); /* copy byte from kernel space to user space */
		count++;
		len--;
		readPos++;
	}
	return count;
}


/**************************************************************/
/* called when 'write' system call is done on the device file */
/**************************************************************/

static ssize_t device_write(struct file *filep, const char *buff, size_t len, loff_t *off)
{
   short ind = len -1;
   short count = 0;
   printk("CHARDEV_DEBUG file: %s, func: %s, line %d\n", __FILE__,__func__,__LINE__);   
   memset(msg,0,100);
   readPos = 0;
   while(len >0) 
   {
      msg[count++] = buff[ind--];
      len--;
   }

   return count;
}

/**************************************************************/
/* called when 'ioctl' system call is done on the device file */
/**************************************************************/

static long device_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
    printk("CHARDEV_DEBUG file: %s, func: %s, line %d\n", __FILE__,__func__,__LINE__);

switch(cmd)
{
    case GET_VAR:
        printk("\n GET_VAR \n");
        
        if (copy_to_user((int *)args, &ker_var, sizeof(int)))
        {
            return -EACCES;
        }
        printk("ker_var: %d\n", ker_var); 
        break;

    case SET_VAR:
         printk("\n SET_VAR \n");
        if (copy_from_user(&ker_var, (int *)args, sizeof(int)))
        {
            return -EACCES;
        }        
        printk("ker_var: %d\n", ker_var); 
        break;

    case CLR_VAR:
        printk("\n CLR_VAR \n");
        ker_var = 0; 
        printk("ker_var: %d\n", ker_var); 
        break;

    default:
        return -EINVAL;
}
return 0;
}

/**************************************************************/
/* called when 'close' system call is done on the device file */
/**************************************************************/

static int device_release(struct inode *inodep, struct file *filep)
{
   printk("CHARDEV_DEBUG file: %s, func: %s, line %d\n", __FILE__,__func__,__LINE__); 
   printk(KERN_ALERT "Device closed !\n");
   return 0;
}

