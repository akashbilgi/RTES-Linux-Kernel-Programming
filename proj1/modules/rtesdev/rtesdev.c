#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/ioctl.h>

#define IOCTL_PRINT_HELLO _IO(0, 0) 
#define IOCTL_GET_TIME_NS _IO(0, 1) 

/*
** This function will be called when we open the Misc device file
*/

/*
** This function will be called when we read the Misc Device file
*/
static ssize_t etx_misc_read(struct file *filp, char __user *buf,
                    size_t count, loff_t *f_pos)
{
    char task_info[256];
    struct task_struct *task;
    ssize_t len = 0;

    for_each_process(task)
    {
        if (task->prio > 0)
        {
            len += snprintf(task_info + len, sizeof(task_info) - len,
                            "Task ID (pid): %d, Process ID (tgid): %d, Real-time priority: %d, Command name: %s\n",
                            task->pid, task->tgid, task->prio, task->comm);
        }
    }
    pr_info("EtX misc device read\n");
 
    return simple_read_from_buffer(buf, count,f_pos, task_info, len);
}
static long ioctl_get(struct file *file, unsigned int cmd, unsigned long arg)
{
ktime_t current_time;
switch(cmd) {
	case IOCTL_PRINT_HELLO:
		pr_info( "Hello world! team13 in kernel space");
		break;
	case IOCTL_GET_TIME_NS:
 		current_time = ktime_get();
    		return copy_to_user((void __user *)arg, &current_time, sizeof(ktime_t)) ? -EFAULT : 0; 
}
return 0;
}
static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    
    .read           = etx_misc_read,

    .unlocked_ioctl  = ioctl_get, 
};
//Misc device structure
struct miscdevice etx_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "simple_etx_misc",
    .fops = &fops,
};
/*
** Misc Init function
*/
static int __init misc_init(void)
{
    int error;
 
    error = misc_register(&etx_misc_device);
    if (error) {
        pr_err("misc_register failed!!!\n");
        return error;
    }
 
    pr_info("misc_register init done!!!\n");
    return 0;
}
/*
** Misc exit function
*/
static void __exit misc_exit(void)
{
    misc_deregister(&etx_misc_device);
    pr_info("misc_register exit done!!!\n");
}
 
module_init(misc_init);
module_exit(misc_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("team13");
