#include <fb.h>
#include <fbcon.h>
#include <acpi.h>
#include <hpet.h>
#include <pic.h>
#include <serial.h>
#include <util.h>
#include <common.h>
#include <asmfunc.h>
#include <idt.h>
#include <handler.h>

/* Information by bootloader */
struct platform_info {
    struct framebuffer fb;
    void *rsdp;
} __attribute__ ((packed));

void handler(void);

void
start_kernel(void *_reserved1 __attribute__ ((unused)), struct platform_info *pi,
             void *_reserved2 __attribute ((unused)))
{
    struct IDTR *idtr;

    /* screen initalize */
    fb_init(&pi->fb);
    set_fg(255, 255, 255);
    set_bg(0, 70, 250);
    clear_screen();

    init_acpi(pi->rsdp);

    /* Descriptors initalize */
    idtr = init_idt();
    idt_load();

    /* Devices initalize */
    init_pic();
    init_hpet();
    init_serial();

    /* exception handler */
    set_intr_gate(0, intr_de);
    set_intr_gate(1, intr_db);
    set_intr_gate(2, intr_nmi);
    set_intr_gate(3, intr_bp);
    set_intr_gate(4, intr_of);
    set_intr_gate(5, intr_br);
    set_intr_gate(6, intr_ud);
    set_intr_gate(8, intr_df);
    set_intr_gate(9, intr_cso);
    set_intr_gate(10, intr_ts);
    set_intr_gate(11, intr_np);
    set_intr_gate(12, intr_ss);
    set_intr_gate(13, intr_gp);
    set_intr_gate(16, intr_mf);
    set_intr_gate(17, intr_ac);
    set_intr_gate(18, intr_mc);
    set_intr_gate(19, intr_xm);
    set_intr_gate(20, intr_ve);
    set_intr_gate(30, intr_sx);

    puts("HELLO WORLD!\r\n");
    //puts_serial("HELLO WORLD!\n");

    puts("WAIT...");
    alert(5 * SEC_TO_US, handler);

    sti();

    while(1)
        hlt();

    while(1) {
        puts_serial("algebrios> ");
    }
}

void
handler(void)
{
    puts(" DONE");
}

void
isr_exception(uint32_t vec, uint64_t rip, uint64_t cs, uint64_t rflags,
              uint64_t rsp)
{
    puts("!Exception!\r\n");
    puts("vec: ");
    puth(vec, 11);
    puts("\r\n");
    puts("rip: ");
    puth(rip, 11);
    puts("\r\n");
    puts("cs: ");
    puth(cs, 11);
    puts("\r\n");
    puts("rflags: ");
    puth(rflags, 11);
    puts("\r\n");
    puts("rsp: ");
    puth(rsp, 11);
    puts("\r\n");
    puts("GOOD BYE!!!!!!\r\n");

    while(1)
        hlt();
}

void
isr_exception_werror(uint32_t vec, uint64_t error, uint64_t rip, uint64_t cs,
                     uint64_t rflags, uint64_t rsp)
{
    puts("!Exception!\r\n");
    puts("vec: ");
    puth(vec, 11);
    puts("\r\n");
    puts("error: ");
    puth(error, 11);
    puts("\r\n");
    puts("rip: ");
    puth(rip, 11);
    puts("\r\n");
    puts("cs: ");
    puth(cs, 11);
    puts("\r\n");
    puts("rflags: ");
    puth(rflags, 11);
    puts("\r\n");
    puts("rsp: ");
    puth(rsp, 11);
    puts("\r\n");
    puts("GOOD BYE!!!!!!\r\n");

    while(1)
        hlt();
}





