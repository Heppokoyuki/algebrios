#include <util.h>
#include <serial.h>

void
panic(char *s)
{
    puts_serial("KERNEL PANIC!!!\n");
    puts_serial(s);

    while(1) {
        hlt();
    }
}

