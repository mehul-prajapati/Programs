/*
 *  Author: Mehul Prajapati <mehulprajapati2802@gmail.com>
 */

#include <linux/module.h>
#include <linux/init.h>



static int __init my_init(void)
{
	printk(KERN_DEBUG "Hello World!\n");

	return 0;
}
static void __exit my_exit(void)
{
	printk(KERN_DEBUG "Bye bye World\n");

}


MODULE_LICENSE("Dual BSD/GPL");
module_init(my_init);
module_exit(my_exit);

