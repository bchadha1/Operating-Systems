#include<linux/module.h>
#include<linux/miscdevice.h>
#include<asm/uaccess.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/sched.h>
#include <linux/sched/signal.h>
#include<linux/slab.h>

#define DEVICE_NAME "process_list"

MODULE_AUTHOR("Bhuvan Chadha");
MODULE_DESCRIPTION("Process List Character Device");
MODULE_LICENSE("GNU-GPL3");

#define BUFFER_SIZE 250

// required function declarations
static char* get_state(long);
static int open_process_list(struct inode*,struct file *);
static int close_process_list(struct inode*, struct file *);
static ssize_t read_process_list(struct file *file, char __user * out, size_t size, loff_t * off);

//file operations
static struct file_operations process_list_fops = {
	.owner = THIS_MODULE,	
	.open = open_process_list,
	.read = read_process_list,
	.release = close_process_list
};


// data structure for device
static struct miscdevice process_list_device  = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &process_list_fops
};


// deregister device
static void __exit proc_list_exit(void) {
	printk(KERN_INFO "Exiting from the Process List KO\n");
	misc_deregister(&process_list_device);
}

// pointer to current process of task structure
static struct task_struct * curr; 

// misc device driver
static int __init proc_list_init(void)
{
	printk(KERN_INFO "Initializing the Process List KO\n");
	int reg_status;
	reg_status = misc_register(&process_list_device);
	if(reg_status<0){
		printk(KERN_ALERT "Registration FAILED!");
		return 0;
	}
	curr=next_task(&init_task);
	return 0;
}

// open function
static int open_process_list(struct inode * inode,struct file * file) {
	printk(KERN_INFO "Device Opened\n");
	curr=next_task(&init_task);
	return 0;
}


// read function
static ssize_t read_process_list(struct file *file, char *out, size_t size, loff_t * off)
{
	int status;
	char proc[BUFFER_SIZE];
	struct task_struct * task;
	
	memset(proc,0,sizeof(char)*BUFFER_SIZE);
	printk(KERN_INFO "Reading the Prcoess Information...\n");

	for_each_process(task){
		if(curr==task){
			char* state=get_state(task->state);
			memset(proc,0,sizeof(char)*BUFFER_SIZE);
			sprintf(proc, "PID=%d PPID=%d CPU=%d STATE=%s",task->pid,task->parent->pid,task_cpu(task),state);
			printk(KERN_INFO "Sending: %s",proc);
			status = raw_copy_to_user(out, proc, strlen(proc));
			if(status !=0)
			{	
				printk(KERN_ALERT "Error in copying data to user!");
				// bad address error code
				return -EFAULT;
			}
			curr=next_task(task);
			break;
		}	
	}
	return strlen(proc);
}

static int close_process_list(struct inode * inode,struct file *file) {
	printk(KERN_INFO "Device Closed!\n");
	curr = &init_task;
	return 0;
}


char* get_state(long state)
{
	switch(state)
	{
		case 0:
			return "TASK_RUNNING";
		case 1:
			return "TASK_INTERRUPTIBLE";
		case 2:
			return "TASK_UNINTERRUPTIBLE";
		case 3:
			return "TASK_INTERRUPTIBLE , TASK_UNINTERRUPTIBLE";
		case 4:
			return "__TASK_STOPPED";
		case 8:
			return "__TASK_TRACED";
		case 15:
			return "TASK_NORMAL , __TASK_STOPPED , __TASK_TRACED";
		case 16:
			return "EXIT_DEAD";
		case 32:
			return "EXIT_ZOMBIE";
		case 63:
			return "TASK_RUNNING , TASK_INTERRUPTIBLE , TASK_UNINTERRUPTIBLE , __TASK_STOPPED , __TASK_TRACED , EXIT_ZOMBIE , EXIT_DEAD";
		case 64:
			return "TASK_DEAD";
		case 96:
			return "EXIT_ZOMBIE , EXIT_DEAD";
		case 128:
			return "TASK_WAKEKILL";	
		case 130:
			return "TASK_WAKEKILL , TASK_UNINTERRUPTIBLE";
		case 132:
			return "TASK_WAKEKILL , __TASK_STOPPED";
		case 136:
			return "TASK_WAKEKILL , __TASK_TRACED";
		case 256:
			return "TASK_WAKING";
		case 512:
			return "TASK_PARKED";
		case 1024:
			return "TASK_NOLOAD";
		case 1026:
			return "TASK_UNINTERRUPTIBLE , TASK_NOLOAD";
		case 2048:
			return "TASK_NEW";
		case 4096:
			return "TASK_STATE_MAX";
		default:
			return "INVALID";
	}
}

module_init(proc_list_init);
module_exit(proc_list_exit);
