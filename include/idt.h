#ifndef _IDT_H_
#define _IDT_H_

#include <types.h>

#define IDT_ADDR 0x00076000
#define MAX_INTR_NO 256
#define SS_KERNEL_CODE 0x0008
#define SS_KERNEL_DATA 0x0010

#define IDT_PRESENT 0x80
#define IDT_INTGATE 0x0e
#define IDT_TRAPGATE 0x0f

struct IDT_descriptor {
    uint16_t offset_15_0;
    uint16_t segment_selector;
    uint8_t _reserved1;
    uint8_t flags;
    uint16_t offset_31_16;
    uint32_t offset_63_32;
    uint32_t _reserved2;
} __attribute__ ((packed));

struct IDTR {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed));

void set_idt_desc(uint8_t intr_no, void *handler);
struct IDTR *init_idt(void);
void idt_load(void);

#endif
