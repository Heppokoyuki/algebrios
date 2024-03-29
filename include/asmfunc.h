#ifndef _ASM_FUNC_H_
#define _ASM_FUNC_H_

#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
uint32_t inl(uint16_t port);
void outl(uint16_t port, uint32_t value);
void hlt(void);
void sti(void);
void lgdt(void *gdtr, uint64_t selector);
void lidt(void *idtr);
void sidt(void *idtr);
void set_cr3(uint64_t cr3);


#endif
