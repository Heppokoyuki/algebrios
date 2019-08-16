#include <pic.h>
#include <stdint.h>
#include <asmfunc.h>

#define MPIC_ICW1_ADDR	0x0020
#define MPIC_OCW2_ADDR	0x0020
#define MPIC_ICW2_ADDR	0x0021
#define MPIC_ICW3_ADDR	0x0021
#define MPIC_ICW4_ADDR	0x0021
#define MPIC_OCW1_ADDR	0x0021
#define MPIC_IMR_ADDR	0x0021
#define SPIC_ICW1_ADDR	0x00a0
#define SPIC_ICW2_ADDR	0x00a1
#define SPIC_ICW3_ADDR	0x00a1
#define SPIC_ICW4_ADDR	0x00a1
#define SPIC_OCW1_ADDR	0x00a1

#define PIC_OCW2_BIT_MANUAL_EOI	0x60

#define INTR_NO_BASE_MASTER	32
#define INTR_NO_BASE_SLAVE	40

void
init_pic(void)
{
    outb(MPIC_ICW1_ADDR, 0x11);
    outb(MPIC_ICW2_ADDR, INTR_NO_BASE_MASTER);
    outb(MPIC_ICW3_ADDR, 0x04);
    outb(MPIC_ICW4_ADDR, 0x01);
    outb(MPIC_OCW1_ADDR, 0xff);

    outb(SPIC_ICW1_ADDR, 0x11);
    outb(SPIC_ICW2_ADDR, INTR_NO_BASE_SLAVE);
    outb(SPIC_ICW3_ADDR, 0x02);
    outb(SPIC_ICW4_ADDR, 0x01);
    outb(SPIC_OCW1_ADDR, 0xff);
}

void
enable_pic_intr(uint8_t intr_no)
{
    uint8_t ir_no = intr_no - INTR_NO_BASE_MASTER;
    uint8_t ir_bit = 1U << ir_no;
    uint8_t mask = inb(MPIC_IMR_ADDR);

    if(!(ir_bit & mask))
        return;

    outb(MPIC_OCW1_ADDR, mask - ir_bit);
}

void
set_pic_eoi(uint8_t intr_no)
{
    uint8_t ir_no = intr_no - INTR_NO_BASE_MASTER;

    outb(MPIC_OCW2_ADDR, PIC_OCW2_BIT_MANUAL_EOI | ir_no);
}

