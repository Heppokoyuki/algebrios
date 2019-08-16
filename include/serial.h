#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdint.h>

#define COM1_UART 0x3f8

void init_serial(void);

uint8_t serial_received(void);
char read_serial(void);
char getc_serial(void);
int gets_serial(char *buf);

uint8_t is_transmit_empty(void);
void write_serial(char a);
void putc_serial(char a);
void puts_serial(char *a);

#endif

