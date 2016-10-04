#include "h/kernel.h"

// ====================================================================
void gdt_set_gate( uint32_t num, ulong_t base, ulong_t limit, uint8_t access, uint8_t gran) {

    // Setup a descriptor in the Global Descriptor Table
    // ----------------------------------------------------------------
    // Setup the descriptor base address
    GDT[num].base_low = (base & 0xFFFF);
    GDT[num].base_middle = ((base >> 16) & 0xFF);
    GDT[num].base_high = (base >> 24) & 0xFF;

    // Setup the descriptor limits
    GDT[num].limit_low = (limit & 0xFFFF);
    GDT[num].granularity = ((limit >> 16) & 0x0F);

    // Finally, set up the granularity and access flags
    GDT[num].granularity |= (gran & 0xF0);
    GDT[num].access = access;
    // ----------------------------------------------------------------


    /*
    Bits 56-63: Bits 24-32 of the base address
    Bit 55: Granularity
        0: None
        1: Limit gets multiplied by 4K
    Bit 54: Segment type
        0: 16 bit
        1: 32 bit
    Bit 53: Reserved-Should be zero
    Bits 52: Reserved for OS use
    Bits 48-51: Bits 16-19 of the segment limit
    Bit 47 Segment is in memory (Used with Virtual Memory)
    Bits 45-46: Descriptor Privilege Level
        0: (Ring 0) Highest
        3: (Ring 3) Lowest
    Bit 44: Descriptor Bit
        0: System Descriptor
        1: Code or Data Descriptor
    Bits 41-43: Descriptor Type
        Bit 43: Executable segment
            0: Data Segment
            1: Code Segment
        Bit 42: Expansion direction (Data segments), conforming (Code Segments)
        Bit 41: Readable and Writable
            0: Read only (Data Segments); Execute only (Code Segments)
            1: Read and write (Data Segments); Read and Execute (Code Segments)
    Bit 40: Access bit (Used with Virtual Memory)
    Bits 16-39: Bits 0-23 of the Base Address
    Bits 0-15: Bits 0-15 of the Segment Limit
    */

    // ----------------------------------------------------------------

}

void gdt_install() {

    // ----------------------------------------------------------------
    // [0x00000000 >> 0x7fffffff] = 2147483647 ÷ 1024 ÷ 1024 = 2047.999999046 MiB
    // ----------------------------------------------------------------
    // *C* printf("[::0]\n" ); usleep(500);
    // Setup the GDT pointer and limit
    GP.limit = (sizeof(_GDT_ENTRY) * _GDT_ENTRY_TTL) - 1;
    GP.base = (uint32_t) &GDT;

    // --------------------------------------------
    // Our NULL descriptor
    uint32_t num      = 0;
    ulong_t base      = 0;
    ulong_t limit     = 0;
    uint8_t access    = 0;
    uint8_t gran      = 0;

    // *C* printf("[::1]\n" ); usleep(500);
    gdt_set_gate(num++, base, limit, access, gran);

    // --------------------------------------------
    // [CS] Code Segment. base=0, limit=4GBytes, 4KByte=granularity uses 32-bit opcodes, and is a Code Segment descriptor.

    // [BIT]  [1024×1024×10] × 8 = 167772160 = 0xA000000
    // [BYTE] [1024×1024×10] = 10485760 = 0xA00000

    // 1024×1024×2 = 2097152 = 0x00200000 == 2MiB

    //base    = 0;// 0
    //limit   = 0x04ffffff;// 4MiB 0x03ffffff;

    base    = 0;
    limit   = 0x03ffffff;
    access  = 0x9A;
    gran    = 0xCF;

    // *C* printf("[::2]\n" ); usleep(500);
    gdt_set_gate(num++, base, limit, access, gran);
    // --------------------------------------------
    // [DT] The third entry is our Data Segment. It's EXACTLY the same as our code segment, but the descriptor type in this entry's access byte says it's a Data Segment

    // [BIT]  [1024×1024×20] × 8 = 335544320 = 0x14000000
    // [BYTE] [1024×1024×20] = 20971520 = 0x1400000
    // 1024×1024×4 = 4194304 = 0x00400000 == 4MiB

    //base    = 0; // 0
    //limit   *= 2;// 4MiB 0x03ffffff;

    base    = 0;
    limit   = 0x03ffffff;
    access  = 0x92;
    gran    = 0xCF;

    // *C* printf("[::3]\n" ); usleep(500);
    gdt_set_gate( num++, base, limit, access, gran );
    // *C* printf("[::4]\n" ); usleep(500);

    // ----------------------------------------------------------------


}

// ====================================================================
void ___gdt_flush___() {

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


void encodeGdtEntry(uint8_t *target, struct GDT source) {

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