#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/hugetlb.h>
#include <linux/swap.h>
#include <linux/swapops.h>
#include <linux/printk.h>

bool page_vma_mapped_walk(struct page_vma_mapped_walk *pvmw)
{

    unsigned long pfn;
    struct pid*pid;
    struct task_struct*pid_struct;
    struct mm_struct *mm = pvmw->vma->vm_mm;
    struct page *page = pvmw->page;
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t pmde;

    pgd = pgd_offset(mm, pvmw->address);
    if (!pgd_present(*pgd))
	return false;
	
    p4d = p4d_offset(pgd, pvmw->address);
    if (!p4d_present(*p4d))
	return false;
	
    pud = pud_offset(p4d, pvmw->address);
    if (!pud_present(*pud))
	return false;
	
    pvmw->pmd = pmd_offset(pud, pvmw->address);
    pmde = READ_ONCE(*pvmw->pmd);
    if (!pmd_present(pmde))
	return false;

    pvmw->pte = pte_offset_map(pvmw->pmd, pvmw->address);
    if (!pte_present(*pvmw->pte))
	return false;
	
    pfn = pte_pfn(*pvmw->pte);

    printk(KERN_INFO "Page frame number", page);
}
