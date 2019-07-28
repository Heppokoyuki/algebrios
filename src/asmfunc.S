    .globl inb
    .globl outb
    .globl hlt

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
    
