#ifndef _KERNEL_TIMES_H_
#define _KERNEL_TIMES_H_

#include "kernel.h"

// ====================================================================
uint64_t kernel_ticks;

void timer_wait( uint32_t ticks );
void timer_handler( REG *reg );
void timer_install();













// ====================================================================
#endif