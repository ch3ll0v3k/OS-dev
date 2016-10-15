#include "h/kernel.h"

// ====================================================================
void gdt_set_gate( uint32 num, ulong base, ulong limit, uint8 access, uint8 gran) {

    // Setup a descriptor in the Global Descriptor Table
    // ----------------------------------------------------------------
    // Setup the descriptor base address
    GDT[num].base_low = (base & 0xFFFF);
    GDT[num].base_middle = (base >> 16) & 0xFF;
    GDT[num].base_high = (base >> 24) & 0xFF;

    // Setup the descriptor limits
    GDT[num].limit_low = (limit & 0xFFFF);
    GDT[num].granularity = ((limit >> 16) & 0x0F);

    // Finally, set up the granularity and access flags
    GDT[num].granularity |= (gran & 0xF0);
    GDT[num].access = access;
    // ----------------------------------------------------------------

}

void gdt_install() {

    // ----------------------------------------------------------------
    // *C* printf("[::0]\n" ); _usleep(500);
    // Setup the GDT pointer and limit
    GP.limit = (sizeof(_GDT_ENTRY) * _GDT_ENTRY_TTL) - 1;
    GP.base = (uint32) &GDT;

    // --------------------------------------------
    // Our NULL descriptor
    uint32 num      = 0;
    ulong base      = 0;
    ulong limit     = 0;
    uint8 access    = 0;
    uint8 gran      = 0;

    // *C* printf("[::1]\n" ); _usleep(500);
    gdt_set_gate(num++, base, limit, access, gran);

    // --------------------------------------------
    // [CS] Code Segment. base=0, limit=4GBytes, 4KByte=granularity uses 32-bit opcodes, and is a Code Segment descriptor.

    base    = 0x04000000;
    limit   = 0x03ffffff;
    access  = 0x9A;
    gran    = 0xCF;

    // *C* printf("[::2]\n" ); _usleep(500);
    gdt_set_gate(num++, base, limit, access, gran);
    // --------------------------------------------
    // [DT] The third entry is our Data Segment. It's EXACTLY the same as our code segment, but the descriptor type in this entry's access byte says it's a Data Segment

    base    = 0x08000000;
    limit   = 0x03ffffff;
    access  = 0x92;
    gran    = 0xCF;

    // *C* printf("[::3]\n" ); _usleep(500);
    gdt_set_gate( num++, base, limit, access, gran );
    // *C* printf("[::4]\n" ); _usleep(500);

    // ----------------------------------------------------------------

}

// ====================================================================
void gdt_flush() {

    // ----------------------------------------------------------------
    // lgdt                ; Load the GDT with our '_gp' which is a special pointer
    // *G* printf("[0]\n" );
    //__asm__ __volatile__ ("lgdt (%0)" : : "r" (&GP));


    // *G* printf("[1]\n" );
    // mov ax, 0x10        ; 0x10 is the offset in the GDT to our data segment
    //__asm__ __volatile__ ("mov $0, %%ax" : : "r"(0x10) );

    // *G* printf("[2]\n" );
    // mov ds, ax
    //__asm__ __volatile__ ("mov %%ax, %%dx" : : );

    // mov es, ax
    //__asm__ __volatile__ ("mov %%ax, %%isx" : : );

    // mov fs, ax
    //__asm__ __volatile__ ("mov %%ax, %%ifx" : : );
    // mov gs, ax
    // mov ss, ax
    // jmp 0x08:flush2     ; 0x08 is the offset to our code segment: Far jump!
    // flush2:


    // *G* printf("[3]\n" );
    //     ret             ; Returns back to the C code!
    //__asm__ __volatile__ ("ret" : : );

    // ----------------------------------------------------------------

}

// ====================================================================
void init_gdt_register() {

    // ----------------------------------------------------------------
    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setting GDT");
    gdt_install();

    // --------------------------------------------
    // Flush out the old GDT and install the new changes!
    gdt_flush();

    // ----------------------------------------------------------------

}


// ====================================================================

/*

// You want to have full 4 GiB addresses untranslated: just use
// Note that in this model, code is not actually protected against
// overwriting since code and data segment overlaps.

GDT[0] = {.base=0, .limit=0, .type=0};                     // Selector 0x00 cannot be used
GDT[1] = {.base=0, .limit=0xffffffff, .type=0x9A};         // Selector 0x08 will be our code
GDT[2] = {.base=0, .limit=0xffffffff, .type=0x92};         // Selector 0x10 will be our data
GDT[3] = {.base=&myTss, .limit=sizeof(myTss), .type=0x89}; // You can use LTR(0x18)


// If you want (for specific reason) to have code and data clearly separated
// (let's say 4 MiB for both, starting at 4 MiB as well), just use:

GDT[0] = {.base=0, .limit=0, .type=0};                      // Selector 0x00 cannot be used
GDT[1] = {.base=0x04000000, .limit=0x03ffffff, .type=0x9A}; // Selector 0x08 will be our code
GDT[2] = {.base=0x08000000, .limit=0x03ffffff, .type=0x92}; // Selector 0x10 will be our data
GDT[3] = {.base=&myTss, .limit=sizeof(myTss), .type=0x89};  // You can use LTR(0x18)

// That means whatever you load at physical address 4 MiB will appear as
// code at CS:0 and what you load at physical address 8 MiB will appear
// as data at DS:0. However it might not be the best design.


// \param target A pointer to the 8-byte GDT entry
// \param source An arbitrary structure describing the GDT entry

void encodeGdtEntry(uint8_t *target, struct GDT source)
{
    // Check the limit to make sure that it can be encoded
    if ((source.limit > 65536) && (source.limit & 0xFFF) != 0xFFF)) {
        kerror("You can't do that!");
    }
    if (source.limit > 65536) {
        // Adjust granularity if required
        source.limit = source.limit >> 12;
        target[6] = 0xC0;
    } else {
        target[6] = 0x40;
    }

    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] |= (source.limit >> 16) & 0xF;

    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    // And... Type
    target[5] = source.type;
}



*/