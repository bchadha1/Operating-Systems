#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/sched.h>
#include <linux/types.h>

struct task_struct
{
	...
	struct mm_struct*mm;
	mm = 
	...
};

struct mm_struct
{
	...
	pgd_t*pgd;
	...
}

struct pid*pid;
struct task_struct*pid_struct;
struct mm_struct*pid_mm_struct;

pid=find_get_pid(process_ID);
pid_struct=pid_task(pid, PIDTYPE_PID);
pid_mm_struct=pid_struct->mm;
pgd=pgd_offset(pid_struct->mm, address);

for (vma= mm->mmap; vma; vma= vma->vm_next)
{
	for(vaddr= vma->vm_start; vaddr< vma->vm_end; vaddr++)
	{
		pgd= pgd_offset(mm, vaddr);
		...
		...
	}
}
		
		
		
