#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/compiler.h>
#include <linux/unistd.h>

#define __NR_count_rt_tasks_mod 449

unsigned long *sys_call_table;
EXPORT_SYMBOL(sys_call_table);

asmlinkage long count_rt_tasks_mod(int *result)
{
    int count = 0;
    struct task_struct *task;
    for_each_process(task) {
        if (task->rt_priority > 50)
            count++;
    }
    *result = count;
    return 0;
}

asmlinkage long (*orig_count_rt_tasks)(int *result);

static int __init mod_count_tasks_init(void)
{
    orig_count_rt_tasks = (void *)sys_call_table[__NR_count_rt_tasks_mod];
    sys_call_table[__NR_count_rt_tasks_mod] = (unsigned long)count_rt_tasks_mod;
    return 0;
}

static void __exit mod_count_tasks_exit(void)
{
    sys_call_table[__NR_count_rt_tasks_mod] = (unsigned long)orig_count_rt_tasks;
}
MODULE_LICENSE("GPL");
module_init(mod_count_tasks_init);
module_exit(mod_count_tasks_exit);
