#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

static struct page_vma_mapped_walk (unsigned long *pvmw)
{

    struct pid*pid;
    struct task_struct*pid_struct;
    struct mm_struct *mm = pvmw->vma->vm_mm;
    struct page *page = pvmw->page;
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t pmde;

    struct page *page = NULL;
    struct mm_struct *mm = current->mm;

    pgd = pgd_offset(mm, pvmw->address);
    if (!pgd_present(*pgd))
		return false;
    printk(KERN_NOTICE "Valid pgd");
    
    p4d = p4d_offset(pgd, pvmw->address);
	if (!p4d_present(*p4d))
		return false;

    pud = pud_offset(p4d, pvmw->address);
	if (!pud_present(*pud))
		return false;
    printk(KERN_NOTICE "Valid pud");

    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        goto out;
    printk(KERN_NOTICE "Valid pmd");

    ptep = pte_offset_map(pmd, addr);
    if (!ptep)
        goto out;
    pte = *ptep;
    
    if (!pmd_present(pmde)) {
		return false;

    page = pte_page(pte);
    if (page)
    {
        printk(KERN_INFO "page frame struct is @ %p", page);
    }
    pte_unmap(ptep);

 out:
    return page;
}


pid=find_get_pid(process_ID);
pid_struct=pid_task(pid, PIDTYPE_PID);
pid_mm_struct=pid_struct->mm;
pgd=pgd_offset(pid_struct->mm, address);

//for (vma= mm->mmap; vma; vma= vma->vm_next)
//{
//	for(vaddr= vma->vm_start; vaddr< vma->vm_end; vaddr++)
//	{
//		pgd= pgd_offset(mm, vaddr);
//		...
//		...
//	}
//}
		
		
		
