#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <asm/pgtable.h>

//unsigned long **sys_call_table = (unsigned long **)0xffffffff810a0e00;
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

static void set_writable(void)
{
    unsigned long cr0;

    cr0 = read_cr0();
    clear_bit(16, &cr0);
    write_cr0(cr0);
}

static void set_readonly(void)
{
    unsigned long cr0;

    cr0 = read_cr0();
    set_bit(16, &cr0);
    write_cr0(cr0);
}

static int __init mod_count_tasks_init(void)
{
    //sys_call_table = (unsigned long **)0xffffffff81801400;;
    //set_writable();
    orig_count_rt_tasks = (void *)sys_call_table[449];
    sys_call_table[449] = (unsigned long *)count_rt_tasks_mod;
    //set_readonly();
    return 0;
}

static void __exit mod_count_tasks_exit(void)
{
    set_writable();
    sys_call_table[449] = (unsigned long *)orig_count_rt_tasks;
    //set_readonly();
}
MODULE_LICENSE("GPL");
module_init(mod_count_tasks_init);
module_exit(mod_count_tasks_exit);
