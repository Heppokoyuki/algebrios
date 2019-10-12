#include <hpet.h>
#include <pic.h>
#include <fbcon.h>
#include <stdint.h>
#include <asmfunc.h>

#define SCHED_PERIOD (5 * MS_TO_US)
#define NUM_TASKS 2

uint64_t task_sp[NUM_TASKS];
volatile uint16_t current_task = 0;

void
schedule(void)
{
    set_pic_eoi(HPET_INTR_NO);
    putc('.');
}

void
init_sched(uint64_t current_rsp)
{
    task_sp[current_task] = current_rsp;
    current_task = (current_task + 1) % NUM_TASKS;
    ptimer_setup(SCHED_PERIOD, schedule);
}

void
sched_start(void)
{
    ptimer_start();
}
