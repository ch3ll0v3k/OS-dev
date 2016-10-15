#ifndef _KERNEL_KERNEL_H_
#define _KERNEL_KERNEL_H_


// ====================================================================
#include "kernel_types.h"

#define _KERNEL_ "k_name"
// ====================================================================
// KERNEL CODE SEGMENT
#define KERNEL_CS 0x08

uint8_t KERNEL_BOOT_INFO;
typedef struct {

    uint32_t gs, fs, es, ds;                          // pushed the segs last
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pushed by 'pusha'
    uint32_t int_no, err_code;                        // our 'push byte #' and ecodes do this
    uint32_t eip, cs, eflags, useresp, ss;            // pushed by the processor automatically
} __attribute__ ((packed)) REG;


extern void kernel_main( char* MK_struct, unsigned int MAGIC );

extern void test();
extern void kernel_mknot();
extern void kernel_mknot_2();
extern void IRQ_HANDLER( REG *r);


void *hander;

// KERNEL DATA SEGMENT
//#define KERNEL_DS 0x10

// USER CODE SEGMENT
// * #define USER_CS 0x08

// USER DATA SEGMENT
// * #define USER_DS 0x10


// ====================================================================
#include "kernel_utils.h"
#include "kernel_stdarg.h"
#include "kernel_string.h"
#include "kernel_system.h"
#include "kernel_terminal.h"
#include "kernel_stdio.h"
#include "kernel_isr.h"
#include "kernel_idt.h"
#include "kernel_gdt.h"
#include "kernel_timer.h"
#include "kernel_dev_mod.h"
#include "kernel_os.h"
#include "kernel_ata_pio.h"
#include "kernel_acpi.h"

// ====================================================================
// ====================================================================

#endif
