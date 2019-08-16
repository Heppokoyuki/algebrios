#ifndef _HPET_H_
#define _HPET_H_

#include <stdint.h>

#define MS_TO_US 1000
#define SEC_TO_US 1000000
#define HPET_INTR_NO 32

void init_hpet(void);
void dump_gcidr(void);
void dump_gcr(void);
void dump_mcr(void);
void sleep(uint64_t us);
void alert(uint64_t us, void *handler);
void ptimer_setup(uint64_t us, void *handler);
void ptimer_start(void);
void ptimer_stop(void);

#endif
