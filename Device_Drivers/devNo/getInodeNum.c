//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <linux/init.h>
#include <linux/module.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <linux/kdev_t.h>

//==================================================================
//================= @DEFINES =======================================
//==================================================================
MODULE_LICENSE("Dual BSD/GPL"); /* It is used to tell the kernel that this module bears a free license; without such a declaration, the kernel
				   complains when the module is loaded. */

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
static int __init hello_init(void)
{
	printk(KERN_ALERT "Hello, world\n"); /* The string KERN_ALERT is the priority of the message.*/

	printk(KERN_ALERT "Dev No: %d\n", MKDEV(1, 1));	

	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, amazing world\n");
}

module_init(hello_init); /* module is loaded into the kernel */
module_exit(hello_exit); /* one for when the module is removed */

