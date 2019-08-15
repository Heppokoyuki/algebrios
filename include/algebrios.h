#ifndef _ALGEBRIOS_H_
#define _ALGEBRIOS_H_


// constant memory allocation in kernel
#define KERNEL_RESERVED_GDT_START 0x00074000
#define KERNEL_RESERVED_GDT_END 0x00075fff
#define KERNEL_RESERVED_IDT_START 0x00076000
#define KERNEL_RESERVED_IDT_END 0x00077fff
#define KERNEL_RESERVED_KERNEL_CODE_START 0x00110000
#define KERNEL_RESERVED_KERNEL_CODE_END 0x00200000
#define KERNEL_RESERVED_KERNEL_BASE_PAGE_TABLE_START 0x77cc0000
#define KERNEL_RESERVED_KERNEL_BASE_PAGE_TABLE_END 0x784c0000

#endif
