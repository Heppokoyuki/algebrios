    .globl inb
    .globl outb
    .globl outl
    .globl inl
    .globl cli
    .globl sti
    .globl hlt
    .globl lgdt
    .globl lidt
    .globl sidt
    .globl set_cr3

    .text
    .code64

    /** uint8_t inb(uint16_t port) **/
inb:
    movw %di,%dx 
    xorq %rax, %rax
    inb %dx, %al
    ret

    /** void outb(uint16_t port, uint8_t value) **/
outb:
    movw %di, %dx
    movb %sil, %al
    outb %al, %dx
    ret

    /** uint32_t inl(uint16_t port) **/
inl:
    movw %di, %dx
    xorq %rax, %rax
    inl %dx, %eax
    ret

    /** void outl(uint16_t port, uint32_t value) **/
outl:
    movw %di, %dx
    movl %esi, %eax
    outl %eax, %dx
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

    /** void sidt(void *idtr) **/
sidt:   
    sidt (%rdi)
    ret

    /** void set_cr3(uint64_t cr3) **/
set_cr3:    
    movq %rdi, %cr3
    ret

    /** void set_rsp(uint64_t rsp) **/
set_rsp:
    movq %rdi, %rsp
    ret

