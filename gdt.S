    .globl gdt_init
    .text
    .code32

#define KERNEL_CODE 0x0008
#define KERNEL_DATA 0x0010

gdt_init:
    push %ax
    lgdt (gdtr)

    movw $KERNEL_DATA, %ax /* set to data segment */
    //movw %ax, %ds
    //movw %ax, %ss

    //ljmpl $KERNEL_CODE, $ret_label /* set to code segment */
ret_label:
    pop %ax
    ret

    .data

    .word 0x0
gdt:
    .word 0x0, 0x0, 0x0, 0x0 /* null descriptor */
    .word 0xffff, 0x0, 0x9a00, 0xaf /* base=0, limit=4GB, mode=code(rx), kernel */
    .word 0xffff, 0x0, 0x9300, 0xcf /* base=0, limit=4GB, mode=data(rw), kernel */
gdt.end:
gdtr:
    .word gdt.end - gdt - 1 /* limit */
    .long gdt /* address */

    
