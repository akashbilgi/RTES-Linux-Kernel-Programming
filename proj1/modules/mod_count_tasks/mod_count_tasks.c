#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <asm/syscall.h>



extern sys_call_ptr_t sys_call_table[];

asmlinkage long (*count_tasks)(int*);

SYSCALL_DEFINE1(mod_count_tasks, int*, result){
	int counter=0;
	struct task_struct *task, *thread;
	
	printk("oveerided count task (449) syscall");
	for_each_process_thread(task, thread){
		if(task->rt_priority>50)
		counter++;
	}

	int ret = copy_to_user(result, &counter, sizeof(counter));
	
	if(ret){
		printk("error while copying data");
		return -1;
	}
	printk("succesfully copied data");
	return 0;
}

int init_mod(void){
	//__NR_count_task == 449
	count_tasks = (void*)sys_call_table[__NR_count_tasks];
	
	sys_call_table[__NR_count_tasks] = (void*) __x64_sys_mod_count_tasks;
	return 0;
}
void exit_module(void){
	sys_call_table[__NR_count_tasks] = (void*)count_tasks;
	
}
MODULE_LICENSE("GPL");
module_init(init_mod);
module_exit(exit_module);
