#include "h/kernel.h"

int16_t l_16( int address ) {
    return (uint16_t)((address) & 0xffff);
}

int16_t h_16( int address ) {
    return (uint16_t)((address) >> 16 & 0xffff);

}

// ====================================================================
// This array is actually an array of function pointers. We use
// this to handle custom IRQ handlers for a given IRQ
void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler)( REG *r)) {
    irq_routines[irq] = handler;
}

// This clears the handler for a given IRQ
void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}

void irq_remap(void) {

    // ----------------------------------------------------------------
    if (KERNEL_BOOT_INFO) kernel_log_info(" :: remap IRQs - PICs");
    // Master PIC: 0x20 and 0x21
    // Slave  PIC: 0xA0 and 0xA1
    outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20); outb(0xA1, 0x28);
    outb(0x21, 0x04); outb(0xA1, 0x02);
    outb(0x21, 0x01); outb(0xA1, 0x01);
    outb(0x21, 0x0);  outb(0xA1, 0x0);

    // disable all IRQs
    // */*/* outb(0x21, 0xFF);

    //if (KERNEL_BOOT_INFO) kernel_log_info(" :: reinit interrupts");
    //__asm__ __volatile__( "sti" : : );

    // ----------------------------------------------------------------

}

// ====================================================================
void set_idt_gate( int n, uint32_t h ) {


    IDT_GATES[ n ].l_offset = (uint16_t)((h) & 0xFFFF);    // bit [0..15 ] Lower part of the interrupt function's offset address (also known as pointer).
    IDT_GATES[ n ].selector = 0x08;            // bit [16..31] [a code segment selector in GDT or LDT] Selector of the interrupt function (to make sense - the kernel's selector). The selector's descriptor's DPL field has to be 0.
    IDT_GATES[ n ].zero     = 0;                     // bit [32..39] Unused 0..7     Have to be 0.

    //IDT_GATES[ n ].flags    = 0x8E;             // bit [40..43] Gate Type 0..3
    IDT_GATES[ n ].flags    = 0x8E;             // bit [40..43] Gate Type 0..3

    IDT_GATES[ n ].h_offset = (uint16_t)(h >> 16 & 0xFFFF);    // bit [48..63] Offset 16..31   Higher part of the offset.
    //IDT_GATES[ n ].h_offset = (uint16_t)((h & 0xffff0000) >> 16);    // bit [48..63] Offset 16..31   Higher part of the offset.



}

void set_idt_register() {

    if (KERNEL_BOOT_INFO) kernel_log_info(" : SETUP [IDT]");

    IDT_REGISTER.base  = (uint32_t) &IDT_GATES;
    IDT_REGISTER.limit = ( 64 * IDT_ENTRYS) -1;

    //if (KERNEL_BOOT_INFO) kernel_log_info(" ")

    if (KERNEL_BOOT_INFO) kernel_log_info(" : LOADING [IDT]");
    idt_load();

    //__asm__ __volatile__( "lidtl (%0) " : : "r" (&IDT_REGISTER) );

    __asm__ __volatile__( "sti" : : );

}

void p_0() {
    printf(" ******************* [IRQ] **************** \n");
}

void init_idt_register() {

    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setting ISR: [0..21]");

    set_idt_gate( 0, (uint32_t)isr0 );
    set_idt_gate( 1, (uint32_t)isr1 );
    set_idt_gate( 2, (uint32_t)isr2 );
    set_idt_gate( 3, (uint32_t)isr3 );
    set_idt_gate( 4, (uint32_t)isr4 );
    set_idt_gate( 5, (uint32_t)isr5 );
    set_idt_gate( 6, (uint32_t)isr6 );
    set_idt_gate( 7, (uint32_t)isr7 );
    set_idt_gate( 8, (uint32_t)isr8 );
    set_idt_gate( 9, (uint32_t)isr9 );
    set_idt_gate( 10, (uint32_t)isr10 );
    set_idt_gate( 11, (uint32_t)isr11 );
    set_idt_gate( 12, (uint32_t)isr12 );
    set_idt_gate( 13, (uint32_t)isr13 );
    set_idt_gate( 14, (uint32_t)isr14 );
    set_idt_gate( 15, (uint32_t)isr15 );
    set_idt_gate( 16, (uint32_t)isr16 );
    set_idt_gate( 17, (uint32_t)isr17 );
    set_idt_gate( 18, (uint32_t)isr18 );
    set_idt_gate( 19, (uint32_t)isr19 );
    set_idt_gate( 20, (uint32_t)isr20 );
    set_idt_gate( 21, (uint32_t)isr21 );

    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setting IRQ: [32..47]");

    // install
    irq_remap();

    set_idt_gate(32, (uint32_t) irq0 );
    set_idt_gate(33, (uint32_t) irq1 );
    set_idt_gate(34, (uint32_t) irq2 );
    set_idt_gate(35, (uint32_t) irq3 );
    set_idt_gate(36, (uint32_t) irq4 );
    set_idt_gate(37, (uint32_t) irq5 );
    set_idt_gate(38, (uint32_t) irq6 );
    set_idt_gate(39, (uint32_t) irq7 );
    set_idt_gate(40, (uint32_t) irq8 );
    set_idt_gate(41, (uint32_t) irq9 );
    set_idt_gate(42, (uint32_t) irq10 );
    set_idt_gate(43, (uint32_t) irq11 );
    set_idt_gate(44, (uint32_t) irq12 );
    set_idt_gate(45, (uint32_t) irq13 );
    set_idt_gate(46, (uint32_t) irq14 );
    set_idt_gate(47, (uint32_t) irq15 );

    // --------------------------------------------------
    set_idt_register();

    // --------------------------------------------------

}

// ====================================================================

