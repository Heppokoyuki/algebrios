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
    mov %rdi, %rsp
    call do_hpet_interrupt
    popq %rdi
    popq %rsi
    popq %rbp
    popq %rbx
    popq %rdx
    popq %rcx
    popq %rax
    iretq
