    .globl default_handler
    .globl hpet_handler
    
default_handler:
    jmp default_handler

hpet_handler:
    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rbx
    pushq %rbp
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    mov %rdi, %rsp
    call do_hpet_interrupt
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rbp
    popq %rbx
    popq %rdx
    popq %rcx
    popq %rax
    iretq

    .macro intr_exception_prolog
    pushq %rbp
    movq %rsp, %rbp
    pushq %rax
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %rcx
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %rbx
    .endm

    .macro intr_exception_epilog
    popq %rbx
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rcx
    popq %rdx
    popq %rsi
    popq %rdi
    popq %rax
    popq %rbp
    .endm

    .macro intr_exception_generic name vec
    .globl intr_\name
intr_\name:
    intr_exception_prolog
    movq $\vec, %rdi
    movq 8(%rbp), %rsi
    movq 16(%rbp), %rdx
    movq 24(%rbp), %rcx
    movq 32(%rbp), %r8
    call isr_exception
    intr_exception_epilog
    iretq
    .endm

    .macro intr_exception_werror name vec
    .globl intr_\name
intr_\name:
    intr_exception_prolog
    movq $\vec, %rdi
    movq 8(%rbp), %rsi
    movq 16(%rbp), %rdx
    movq 24(%rbp), %rcx
    movq 32(%rbp), %r8
    movq 40(%rbp), %r9
    call isr_exception_werror
    intr_exception_epilog
    addq $0x8, %rsp
    iretq
    .endm

    intr_exception_generic de 0x00
    intr_exception_generic db 0x01
    intr_exception_generic nmi 0x02
    intr_exception_generic bp 0x03
    intr_exception_generic of 0x04
    intr_exception_generic br 0x05
    intr_exception_generic ud 0x06

    intr_exception_werror df 0x08
    intr_exception_generic cso 0x09
    intr_exception_werror ts 0x0a
    intr_exception_werror np 0x0b
    intr_exception_werror ss 0x0c
    intr_exception_werror gp 0x0d

    intr_exception_generic mf 0x10
    intr_exception_werror ac 0x11
    intr_exception_generic mc 0x12
    intr_exception_generic xm 0x13
    intr_exception_generic ve 0x14
    intr_exception_werror sx 0x1e
