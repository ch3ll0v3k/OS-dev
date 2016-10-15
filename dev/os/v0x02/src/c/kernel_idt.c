#include "h/kernel.h"

int16 l_16( int address ) {
    return (uint16)((address) & 0xffff);
}

int16 h_16( int address ) {
    return (uint16)((address) >> 16 & 0xffff);

}

// ====================================================================
/*void disable_irq(unsigned int irq) {
    unsigned int mask = (1 << irq);
    cached_irq_mask |= mask;
    if (irq & 8) {
        outb(cached_A1,0xA1);
    } else {
        outb(cached_21,0x21);
    }
}*/

/*void enable_irq(unsigned int irq) {
    unsigned int mask = ~(1 << irq);
    cached_irq_mask &= mask;
    if (irq & 8) {
        outb(cached_A1,0xA1);
    } else {
        outb(cached_21,0x21);
    }
}*/

// ====================================================================
/*
#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01

void init_pics(int pic1, int pic2) {
    // send ICW1
    outportb(PIC1, ICW1);
    outportb(PIC2, ICW1);

    // send ICW2
    outportb(PIC1 + 1, pic1);
    outportb(PIC2 + 1, pic2);

    // send ICW3
    outportb(PIC1 + 1, 4);
    outportb(PIC2 + 1, 2);

    // send ICW4
    outportb(PIC1 + 1, ICW4);
    outportb(PIC2 + 1, ICW4);

    // disable all IRQs
    outportb(PIC1 + 1, 0xFF);
}
*/

void init_pic() {

    // ----------------------------------------------------------------
    // Master PIC: 0x20 and 0x21
    // Slave PIC: 0xA0 and 0xA1
    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setup PICs");

    // ----------------------------------------------------------------
    /*uint32 i = 0;
    for ( i = 0; i < 16; i ++ ) {
        disable_irq(i);
        enable_irq(i);
    }
    enable_irq(1);
    set_idt_gate( 0x21, (uint32) isrALL );
    */
    // ----------------------------------------------------------------

    // Initialization of ICW1
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);

    // Initialization of ICW2
    outportb(0x21, 0x20);    // start vector = 32
    outportb(0xA1, 0x70);    // start vector = 96

    // Initialization of ICW3
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);

    // Initialization of ICW4
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);

    // mask interrupts
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);

    // disable all IRQs
    outportb(0x21, 0xFF);


    //if (KERNEL_BOOT_INFO) kernel_log_info(" :: reinit interrupts");
    //__asm__ __volatile__( "sti" : : );

}

// ====================================================================
void set_idt_gate( int n, uint32 h ) {


    IDT_GATES[ n ].l_offset = (uint16)((h) & 0xFFFF);    // bit [0..15 ] Lower part of the interrupt function's offset address (also known as pointer).
    IDT_GATES[ n ].selector = 0x08;            // bit [16..31] [a code segment selector in GDT or LDT] Selector of the interrupt function (to make sense - the kernel's selector). The selector's descriptor's DPL field has to be 0.
    IDT_GATES[ n ].zero     = 0;                     // bit [32..39] Unused 0..7     Have to be 0.

    //IDT_GATES[ n ].flags    = 0x8E;             // bit [40..43] Gate Type 0..3
    IDT_GATES[ n ].flags    = 0x8E;             // bit [40..43] Gate Type 0..3

    IDT_GATES[ n ].h_offset = (uint16)(h >> 16 & 0xFFFF);    // bit [48..63] Offset 16..31   Higher part of the offset.
    //IDT_GATES[ n ].h_offset = (uint16)((h & 0xffff0000) >> 16);    // bit [48..63] Offset 16..31   Higher part of the offset.



}

void set_idt_register() {

    if (KERNEL_BOOT_INFO) kernel_log_info(" :: set_idt_register");

    IDT_REGISTER.base  = (uint32) &IDT_GATES;
    IDT_REGISTER.limit = ( 64 * IDT_ENTRYS) -1;

    //if (KERNEL_BOOT_INFO) kernel_log_info(" ")

    if (KERNEL_BOOT_INFO) kernel_log_info(" :: register __asm__ >>");
    __asm__ __volatile__( "lidtl (%0) " : : "r" (&IDT_REGISTER) );

}

void init_idt_register() {

    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setting ISRs: [0..21]");

    set_idt_gate( 0, (uint32)isr0 );
    set_idt_gate( 1, (uint32)isr1 );
    set_idt_gate( 2, (uint32)isr2 );
    set_idt_gate( 3, (uint32)isr3 );
    set_idt_gate( 4, (uint32)isr4 );
    set_idt_gate( 5, (uint32)isr5 );
    set_idt_gate( 6, (uint32)isr6 );
    set_idt_gate( 7, (uint32)isr7 );
    set_idt_gate( 8, (uint32)isr8 );
    set_idt_gate( 9, (uint32)isr9 );
    set_idt_gate( 10, (uint32)isr10 );
    set_idt_gate( 11, (uint32)isr11 );
    set_idt_gate( 12, (uint32)isr12 );
    set_idt_gate( 13, (uint32)isr13 );
    set_idt_gate( 14, (uint32)isr14 );
    set_idt_gate( 15, (uint32)isr15 );
    set_idt_gate( 16, (uint32)isr16 );
    set_idt_gate( 17, (uint32)isr17 );
    set_idt_gate( 18, (uint32)isr18 );
    set_idt_gate( 19, (uint32)isr19 );
    set_idt_gate( 20, (uint32)isr20 );
    set_idt_gate( 21, (uint32)isr21 );

    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setting ISRs: [22..256]");

    uint16 i = 22;
    for ( ; i < IDT_ENTRYS; i++ ) {
        //set_idt_gate( i, (uint32) isrALL );

    }

    // --------------------------------------------------
    set_idt_register();

    // --------------------------------------------------

}

// ====================================================================

