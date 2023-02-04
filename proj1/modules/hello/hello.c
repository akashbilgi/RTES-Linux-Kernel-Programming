#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */

MODULE_LICENSE("GPL");
MODULE_VERSION("1");

int init_module(void)
{
	printk(KERN_INFO "Hello world! team13 in kernel space[update]\n");
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "\n");
}
