#ifndef _KERNEL_IDT_H_
#define _KERNEL_IDT_H_

#include "kernel.h"
// ====================================================================
#define idt_l_16(addres) (uint16_t)((addres) & 0xffff);
#define idt_h_16(addres) (uint16_t)((addres) >> 16 & 0xffff);


//#define __byte(x,y)     (((uint8_t *)&(y))[x])
//#define cached_21       (__byte(0, cached_irq_mask))
//#define cached_A1       (__byte(1, cached_irq_mask))
//static uint32_t cached_irq_mask = 0xffff;



// ====================================================================
// ["INTERRUPT-DISCRIPTION-TABLE"]
#define IDT_ENTRYS 256
#define IDT_FLAG 0x8E

// http://wiki.osdev.org/Interrupt_Descriptor_Table
typedef struct {

    uint16_t l_offset;    // bit [0..15 ] Lower part of the interrupt function's offset address (also known as pointer).
    uint16_t selector;    // bit [16..31] [a code segment selector in GDT or LDT] Selector of the interrupt function (to make sense - the kernel's selector). The selector's descriptor's DPL field has to be 0.
    uint8_t zero;         // bit [32..39] Unused 0..7     Have to be 0.
    uint8_t flags;        // bit [40..43] Gate Type 0..3
    uint16_t h_offset;    // bit [48..63] Offset 16..31   Higher part of the offset.

} __attribute__((packed)) _IDT_GATE_T;


// Location of IDT (address and size) is kept in the IDTR register
// of the CPU, which can be loaded/stored using LIDT, SIDT instructions.

typedef struct {

    uint16_t limit; // Limit [0..15]   Defines the length of the IDT in bytes - 1 (minimum value is 100h, a value of 1000h means 200h interrupts).
    uint32_t base; // Base  [16..47]  This 32 bits are the linear address where the IDT starts (INT 0)

} __attribute__((packed)) _IDT_REGISTER_T;



_IDT_GATE_T IDT_GATES[ IDT_ENTRYS ];
_IDT_REGISTER_T IDT_REGISTER;

// kernel.asm
extern void idt_load();

void init_idt_register();
void set_idt_gate( int n, uint32_t handler );
void set_idt_register();

void *irq_routines[16];

void irq_remap();
void irq_install_handler( int irq, void (*handler)( REG *r));
void irq_uninstall_handler( int irq);

void p_0();
// ====================================================================

#endif