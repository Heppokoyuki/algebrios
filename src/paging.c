/** 4level paging creation and setup **/

#include <stdint.h>
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

    base = KERNEL_PAGE_TABLE_BASE_ADDRESS;

    /* zero clear all entries */
    for(int i = 0; i < 512; ++i) {
        pml4[i] = 0;
    }

    /*
      +---------------------------------------+
      | index |           address             |
      +---------------------------------------+
      | pml4  |KERNEL_PAGE_TABLE_BASE_ADDRESS |
      +---------------------------------------+
      | pdpt |        pml4 + 0x1000           |
      +---------------------------------------+
      |  pd  |        pml4 + 0x2000           |
      +---------------------------------------+
    */

    /* PML4E */
    pml4 = (uint64_t *)base;
    pml4[0] = (base + 0x1000) | 0x07;

    /* PDPTE */
    pdpt = (uint64_t *)(base + 0x1000);
    for(uint64_t i = 0; i < 4; ++i) {
        pdpt[i] = (base + 0x2000 + 0x1000 * i) | 0x07;
    }

    /* PDE */
    pd = (uint64_t *)(base + 0x2000);
    for(uint64_t i = 0; i < 512 * 4; ++i) {
        pd[i] = (uint64_t)(i * 0x00200000) | 0x083;
    }

    set_cr3(base);
}
