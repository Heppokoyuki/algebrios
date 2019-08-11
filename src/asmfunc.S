    .globl inb
    .globl outb
    .globl sti
    .globl hlt
    .globl lidt
    .globl lgdt

    .text
    .code64

    /** uint8_t inb(uint16_t port) **/
inb:
    xorq %rax, %rax
    inb %dx, %al
    ret

    /** void outb(uint16_t port, uint8_t value) **/
outb:
    movw %di, %dx
    movb %sil, %al
    outb %al, %dx
    ret
    
    /** void hlt(void) **/
hlt:
    hlt

    /** void sti(void) **/
sti:
    sti
    ret

    /** void lgdt(void *gdtr, uint64_t selector) **/
lgdt:
    lgdt (%rdi)
    pushq %rsi
    movabs $1f, %rax
    pushq %rax
    lretq
1:
    movq %rsi, %rax
    addq $8, %rax
    movq %rax, %ds
    movq %rax, %es
    movq %rax, %ss
    ret

    /** void lidt(void *idtr) **/
lidt:
    lidt (%rdi)
    ret


