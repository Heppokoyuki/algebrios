#ifndef _PIC_H_
#define _PIC_H_

#include <types.h>

void init_pic(void);
void enable_pic_intr(uint8_t intr_no);
void set_pic_eoi(uint8_t intr_no);

#endif

