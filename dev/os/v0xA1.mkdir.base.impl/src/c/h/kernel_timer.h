#ifndef _KERNEL_TIMES_H_
#define _KERNEL_TIMES_H_

#include "kernel.h"

// ====================================================================
uint64_t kernel_ticks;

void timer_wait( uint32_t ticks );
void timer_handler( REG *reg );
void timer_install();


void TEST();


void rtc_print_time();
void not_registered_func ();

uint8_t RTC[10];
/*
char *M_DAY[] = {
    "Sat",
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
};
*/


// ====================================================================
#endif