#ifndef _ASM_FUNC_H_
#define _ASM_FUNC_H_

#include <types.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
void hlt(void);
void sti(void);
void lgdt(void *gdtr, uint64_t selector);
void lidt(void *idtr);
void sidt(void *idtr);


#endif
