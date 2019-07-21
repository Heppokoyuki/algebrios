    .globl entry

    .text
    .code64

#define KERNEL_CODE 0x0008
#define KERNEL_DATA 0x0010

entry:
    cli
    
    lgdt (gdtr)

    /* set to data segment */
    movw $KERNEL_DATA, %ax 
    movw %ax, %ds
    movw %ax, %ss

    /* set to code segment */
    pushq %rax
    pushq %rsp
    pushfq
    pushq $KERNEL_CODE
    pushq $ret_label
    iretq
ret_label:
    /* Go to kernel! */
    call start_kernel

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

