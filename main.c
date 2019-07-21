#include <fb.h>
#include <console.h>
#include <serial.h>

void
start_kernel(void *_t __attribute__ ((unused)), struct framebuffer *_fb,
             void *_fs_start  __attribute__ ((unused)))
{
    char buf[20];

    fb_init(_fb);
    set_fg(255, 255, 255);
    set_bg(0, 70, 250);
    clear_screen();
    init_serial();

    puts("HELLO WORLD!");
    puts_serial("HELLO WORLD!\n");

    while(1) {
        puts_serial("algebrios> ");
        gets_serial(buf);
    }
}

