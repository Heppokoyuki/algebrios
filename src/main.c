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
#include <sched.h>
#include <handler.h>
#include <memory.h>
#include <paging.h>

/* Information by bootloader */

struct platform_info {
    struct framebuffer fb;
    void *rsdp;
    struct memory_map map;
};

void handler(void);

void
start_kernel(void *_reserved1 __attribute__ ((unused)), struct platform_info *pi,
             void *_reserved2 __attribute__ ((unused)))
{
    struct IDTR *idtr;
    page_frame_mannager_t pfm;
    uint64_t mem_desc_num;
    uint64_t free_page;

    int *test;

    /* screen initalize */
    fb_init(&pi->fb);
    set_fg(0, 0, 0); /* white */
    set_bg(255, 255, 255); /* black */
    clear_screen();

    /* kernel page table setup */
    init_kernel_page_table();

    init_acpi(pi->rsdp);

    /* Descriptors initalize */
    idtr = init_idt();
    idt_load();

    /* Devices initalize */
    init_pic();
    init_hpet();
    init_serial();

    /* Scheduler initalize */
    init_sched();

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

    /* kernel memory manager init */
    mem_desc_num = pi->map.mmap_size / pi->map.mem_desc_unit_size;
    free_page = init_phys_memory(mem_desc_num, pi->map.mem_desc_unit_size, (mdesc_t *)pi->map.mem_desc, &pfm);

    puts("FREE PAGE COUNT ");
    puth(free_page, 10);
    puts("\r\n");
    puts("FREE MEMORY ");
    putd(free_page * MEMORY_PAGESIZE / 1024 / 1024, 5);
    puts("MiB\r\n\r\n");

    dump_phys_memory_page_block(&pfm);

    test = pfalloc(3, &pfm);

    puts("ALLOCATED ");
    puth(test, 10);
    puts("\r\n");

    dump_phys_memory_page_block(&pfm);

    puts("HELLO WORLD!\r\n");
    puts_serial("HELLO WORLD!");
    puts_serial("HELLO WORLD!");
    puts_serial("HELLO WORLD!");

    sti();

    //sched_start();

    while(1)
        hlt();
}

void
handler(void)
{
    static unsigned char counter = 0;
    if(counter < 10)
        putc('0' + counter++);
    else
        ptimer_stop();
}

void
isr_exception(uint32_t vec, uint64_t rip, uint64_t cs, uint64_t rflags,
              uint64_t rsp)
{
    puts("!EXCEPTION!\r\n");
    puts("VEC: ");
    puth(vec, 11);
    puts("\r\n");
    puts("RIP: ");
    puth(rip, 11);
    puts("\r\n");
    puts("CS: ");
    puth(cs, 11);
    puts("\r\n");
    puts("RFLAGS: ");
    puth(rflags, 11);
    puts("\r\n");
    puts("RSP: ");
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
    puts("!EXCEPTION!\r\n");
    puts("VEC: ");
    puth(vec, 11);
    puts("\r\n");
    puts("ERROR: ");
    puth(error, 11);
    puts("\r\n");
    puts("RIP: ");
    puth(rip, 11);
    puts("\r\n");
    puts("CS: ");
    puth(cs, 11);
    puts("\r\n");
    puts("RFLAGS: ");
    puth(rflags, 11);
    puts("\r\n");
    puts("RSP: ");
    puth(rsp, 11);
    puts("\r\n");
    puts("GOOD BYE!!!!!!\r\n");

    while(1)
        hlt();
}

