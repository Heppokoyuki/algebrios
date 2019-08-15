/** 4level paging creation and setup **/

#include <types.h>
#include <paging.h>
#include <fbcon.h>
#include <asmfunc.h>

void
init_kernel_page_table(void)
{
    uint64_t base;
    uint64_t *pml4;
    uint64_t *pdpt;
    uint64_t *pd;
    uint64_t *pt;

    base = KERNEL_PAGE_TABLE_BASE_ADDRESS;

    /* PML4E */
    pml4 = (uint64_t *)base;
    /* zero clear PML4E */
    for(int i = 0; i < 512; ++i)
        pml4[i] = 0;
    pml4[0] = (base + 0x1000) | 0x07;

    /* PDPTE */
    pdpt = (uint64_t *)(base + 0x1000);
    /* zero clear PDPTE */
    for(uint64_t i = 0; i < 512; ++i)
        pdpt[i] = 0;
    for(uint64_t i = 0; i < 4; ++i) {
        pdpt[i] = (base + 0x2000 + 0x1000 * i) | 0x07;
    }

    /* PDE */
    pd = (uint64_t *)(base + 0x2000);
    for(uint64_t i = 0; i < 512 * 4; ++i) {
        pd[i] = (base + 0x6000 + 0x1000 * i) | 0x07;
    }

    /* PTE */
    pt = (uint64_t *)(base + 0x6000);
    for(uint64_t i = 0; i < 512 * 512 * 4; ++i) {
        pt[i] = (0x1000 * i) | 0x07;
    }

    set_cr3(base);
}
