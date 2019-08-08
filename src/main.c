#include <fb.h>
#include <fbcon.h>
#include <acpi.h>
#include <hpet.h>
#include <serial.h>
#include <util.h>
#include <common.h>
#include <idt.h>

/* Information by bootloader */
struct platform_info {
    struct framebuffer fb;
    void *rsdp;
} __attribute__ ((packed));

void
start_kernel(void *_t __attribute__ ((unused)), struct platform_info *pi,
             void *_fs_start)
{
    struct IDTR *idtr;

    /* screen initalize */
    fb_init(&pi->fb);
    set_fg(255, 255, 255);
    set_bg(0, 70, 250);
    clear_screen();

    /* Descriptors initalize */
    idtr = init_idt();
    idt_load();

    /* Devices initalize */
    init_serial();
    init_acpi(pi->rsdp);
    init_hpet();

    puts("WAIT...");
    sleep(5 * SEC_TO_US);
    puts("DONE\r\n");
    while(1);

    puts("HELLO WORLD!");
    puts_serial("HELLO WORLD!\n");

    while(1) {
        puts_serial("algebrios> ");
    }
}

