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

//extern void * sys_call_table[];
//EXPORT_SYMBOL(sys_call_table);
unsigned long **sys_call_table;

static unsigned long **find_sys_call_table(void) {
    unsigned long **sctable;
    unsigned long ptr;

    sctable = NULL;
    for ((unsigned long)sys_call_table;
         ptr < 0xfffffffffffff000UL; ptr += sizeof(void *)) {
        unsigned long *p;

        p = (unsigned long *)ptr;
        if (p[__NR_close] == (unsigned long)sys_call_table[__NR_close]) {
            sctable = (unsigned long **)p;
            return sctable;
        }
    }

    return NULL;
}


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

    if (!(sys_call_table = find_sys_call_table()))
        return -1;
    write_cr0(read_cr0() & (~0x10000));
    orig_count_rt_tasks = (void *)sys_call_table[__NR_count_rt_tasks_mod];
    sys_call_table[__NR_count_rt_tasks_mod] = (unsigned long)count_rt_tasks_mod;
    write_cr0(read_cr0() | 0x10000);
    return 0;
}

static void __exit mod_count_tasks_exit(void)
{
    write_cr0(read_cr0() & (~0x10000));
    sys_call_table[__NR_count_rt_tasks_mod] = (unsigned long)orig_count_rt_tasks;
    write_cr0(read_cr0() | 0x10000);
}
MODULE_LICENSE("GPL");
module_init(mod_count_tasks_init);
module_exit(mod_count_tasks_exit);
