#ifndef _KERNEL_GDT_H_
#define _KERNEL_GDT_H_

#include "kernel.h"

// ====================================================================
void init_gdt_register();


/* Defines a GDT entry. We say packed, because it prevents the
*  compiler from doing things that it thinks is best: Prevent
*  compiler "optimization" by packing */
typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;

} __attribute__((packed)) _GDT_ENTRY;

/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
typedef struct {
    uint16_t limit;
    uint32_t base;

} __attribute__((packed)) _GDT_PTR;

/* Our GDT, with 3 entries, and finally our special GDT pointer */

#define _GDT_ENTRY_TTL 3

_GDT_ENTRY GDT[ _GDT_ENTRY_TTL ];
_GDT_PTR GP;


// This will be a function in start.asm. We use this to properly
// reload the new segment registers

// void gdt_flush();
extern void gdt_flush();

/*
; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush();'
global _gdt_flush     ; Allows the C code to link to this
extern _gp            ; Says that '_gp' is in another file
_gdt_flush:
    lgdt [_gp]        ; Load the GDT with our '_gp' which is a special pointer
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
flush2:
    ret               ; Returns back to the C code!
*/

/*
// Setup a descriptor in the Global Descriptor Table
void gdt_set_gate(int num, uint32_t base, uint32_t limit, unsigned char access, unsigned char gran) {
    // Setup the descriptor base address
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    // Setup the descriptor limits
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    // Finally, set up the granularity and access flags
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

// Should be called by main. This will setup the special GDT
// pointer, set up the first 3 entries in our GDT, and then
// finally call gdt_flush() in our assembler file in order
// to tell the processor where the new GDT is and update the
// new segment registers
void gdt_install() {
    // Setup the GDT pointer and limit
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = &gdt;

    // Our NULL descriptor
    gdt_set_gate(0, 0, 0, 0, 0);

    // The second entry is our Code Segment. The base address
    // is 0, the limit is 4GBytes, it uses 4KByte granularity,
    // uses 32-bit opcodes, and is a Code Segment descriptor.
    // Please check the table above in the tutorial in order
    // to see exactly what each value means
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // The third entry is our Data Segment. It's EXACTLY the
    // same as our code segment, but the descriptor type in
    // this entry's access byte says it's a Data Segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Flush out the old GDT and install the new changes!
    gdt_flush();
}

*/

// ====================================================================
#endif
