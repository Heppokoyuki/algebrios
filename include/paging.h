#ifndef _PAGING_H_
#define _PAGING_H_

#include <algebrios.h>

#define KERNEL_PAGE_TABLE_BASE_ADDRESS KERNEL_RESERVED_KERNEL_BASE_PAGE_TABLE_START

void init_kernel_page_table(void);

#endif
