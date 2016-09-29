#ifndef _KERNEL_KERNEL_H_
#define _KERNEL_KERNEL_H_

// ====================================================================
#include "kernel_types.h"

#define _KERNEL_ "k_name"
// ====================================================================
// KERNEL CODE SEGMENT
#define KERNEL_CS 0x08

uint8 KERNEL_BOOT_INFO;

/* This defines what the stack looks like after an ISR was running */
struct regs {
    uint32 gs, fs, es, ds;      /* pushed the segs last */
    uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    uint32 int_no, err_code;    /* our 'push byte #' and ecodes do this */
    uint32 eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */

};

// KERNEL DATA SEGMENT
//#define KERNEL_DS 0x10

// USER CODE SEGMENT
// * #define USER_CS 0x08

// USER DATA SEGMENT
// * #define USER_DS 0x10

// ====================================================================
#include "kernel_stdarg.h"
#include "kernel_string.h"
#include "kernel_system.h"
#include "kernel_terminal.h"
#include "kernel_stdio.h"
#include "kernel_isr.h"
#include "kernel_idt.h"
#include "kernel_gdt.h"

// ====================================================================

#endif
