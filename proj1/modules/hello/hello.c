#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team13"); 

static int __init hello_init(void) { 
    printk(KERN_INFO "Hello world! team13 in kernel space\n");  
    return 0;
} 

static void __exit hello_exit(void){ 
    printk(KERN_INFO "Goodbye world!\n"); 
} 
module_init(hello_init);
module_exit(hello_exit);

