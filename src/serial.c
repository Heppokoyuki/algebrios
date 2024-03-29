#include <serial.h>
#include <stdint.h>
#include <asmfunc.h>
#include <fbcon.h>

#define MAX_STR_BUF	21

void
init_serial(void)
{
    /* Disable all interrupts */
    outb(COM1_UART + 1, 0x00);

    /* Enable DLAB */
    outb(COM1_UART + 3, 0x80);

    /* Set baud rate to 115200 */
    outb(COM1_UART + 0, 0x01);
    outb(COM1_UART + 1, 0x00);

    /* 8bits, no parity, one stop bit */
    outb(COM1_UART + 3, 0x03);

    /* Enable FIFO, clear them, with 14-byte threshold */
    outb(COM1_UART + 2, 0xc7);
    /* IRQs enabled, RTS/DSR set */
    outb(COM1_UART + 4, 0x0b);
}

uint8_t
serial_received(void)
{
   return inb(COM1_UART + 5) & 1;
}

char
read_serial(void)
{
   while (!serial_received());
   return inb(COM1_UART);
}

char
getc_serial(void)
{
    char a = read_serial();
    a = (a == '\r') ? '\n' : a;
    putc_serial(a);
    return a;
}

int
gets_serial(char *buf)
{
    int i = 0;
    unsigned char a;

    do {
        a = getc_serial();
        if(a == '\n') a = '\0';
        buf[++i] = a;
    } while(a);

    return i - 1;
}

uint8_t
is_transmit_empty(void)
{
    return inb(COM1_UART + 5) & 0x40;
}

void
write_serial(char a)
{
    while(!is_transmit_empty());
    outb(COM1_UART, a);
}

void
putc_serial(char a)
{
    if(a == '\n')
        write_serial('\r');
    putc(a);
    write_serial(a);
}

void
puts_serial(char *a)
{
    while(*a)
        putc_serial(*(a++));
}

void puth_serial(unsigned long long val, unsigned char num_digits)
{
	char str[MAX_STR_BUF];

	int i;
	for (i = num_digits - 1; i >= 0; i--) {
		unsigned char v = (unsigned char)(val & 0x0f);
		if (v < 0xa)
			str[i] = '0' + v;
		else
			str[i] = 'A' + (v - 0xa);
		val >>= 4;
    }
    str[num_digits] = '\0';

    puts_serial(str);
}

