#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

static struct page *walk_page_table(unsigned long addr)
{

    struct pid*pid;
    struct task_struct*pid_struct;
    struct mm_struct*pid_mm_struct;
    pgd_t *pgd;
    pte_t *ptep, pte;
    pud_t *pud;
    pmd_t *pmd;

    struct page *page = NULL;
    struct mm_struct *mm = current->mm;

    pgd = pgd_offset(mm, addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        goto out;
    printk(KERN_NOTICE "Valid pgd");

    pud = pud_offset(pgd, addr);
    if (pud_none(*pud) || pud_bad(*pud))
        goto out;
    printk(KERN_NOTICE "Valid pud");

    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        goto out;
    printk(KERN_NOTICE "Valid pmd");

    ptep= pte_offset_map(pmd, addr);
    if (!ptep)
        goto out;
    pte = *ptep;

    page= pte_page(pte);
    if (page)
    {
        printk(KERN_INFO "page frame struct is @ %p", page);
    }
    pte_unmap(ptep);

 out:
    return page;
}

//struct pid*pid;
//struct task_struct*pid_struct;
//struct mm_struct*pid_mm_struct;

//pid=find_get_pid(process_ID);
//pid_struct=pid_task(pid, PIDTYPE_PID);
//pid_mm_struct=pid_struct->mm;
//pgd=pgd_offset(pid_struct->mm, address);

//for (vma= mm->mmap; vma; vma= vma->vm_next)
//{
//	for(vaddr= vma->vm_start; vaddr< vma->vm_end; vaddr++)
//	{
//		pgd= pgd_offset(mm, vaddr);
//		...
//		...
//	}
//}
		
		
		
