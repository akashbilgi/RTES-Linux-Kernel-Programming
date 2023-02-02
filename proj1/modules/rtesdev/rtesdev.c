#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
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
//File operation structure 
static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    
    .read           = etx_misc_read,

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
 
module_init(misc_init)
module_exit(misc_exit)
 
MODULE_LICENSE("GPL");

