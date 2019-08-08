#include <idt.h>
#include <types.h>
#include <asmfunc.h>

#define DESC_TYPE_INTR 14

void default_handler(void);

void
set_idt_gate_desc(struct IDT_descriptor *idt, uint64_t base, uint16_t selector, uint8_t flags)
{
    idt->offset_15_0 = (uint16_t)(base & 0xffff);
    idt->segment_selector = selector;
    idt->_reserved1 = 0;
    idt->flags = flags;
    idt->offset_31_16 = (uint16_t)((base & 0xffff0000) >> 16);
    idt->offset_63_32 = (uint16_t)((base & 0xffffffff00000000) >> 32);
    idt->_reserved2 = 0;
}

void
set_intr_gate(uint8_t intr_no, void *handler)
{
    struct IDT_descriptor *idt;

    idt = (struct IDT_descriptor *)(IDT_ADDR + intr_no * sizeof(struct IDT_descriptor));
    set_idt_gate_desc(idt, (uint64_t)handler, SS_KERNEL_CODE, IDT_PRESENT | IDT_INTGATE);
}

struct IDTR *
init_idt(void)
{
    struct IDTR *idtr;
    int i;

    for(i = 0; i < MAX_INTR_NO; ++i)
        set_intr_gate(i, default_handler);

    idtr = (struct IDTR *)(IDT_ADDR + sizeof(struct IDT_descriptor) * MAX_INTR_NO);
    idtr->base = IDT_ADDR;
    idtr->limit = MAX_INTR_NO * sizeof(struct IDT_descriptor) - 1;

    return idtr;
}

void
idt_load(void)
{
    struct IDTR *idtr;

    idtr = (struct IDTR *)(IDT_ADDR + sizeof(struct IDT_descriptor) * MAX_INTR_NO);
    lidt(idtr);
}

