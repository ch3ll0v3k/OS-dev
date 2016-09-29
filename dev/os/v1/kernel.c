#include "include/kernel_types.h"
#include "include/kernel.h"

// ====================================================================
uint32 _term_cursor = 0;
void _printf( char* str ) {

    // --------------------------------------------------
    //If we want to make loanger jump
    /*
    uint8 *VID_MEM = (uint8 *)0xb8000;
    uint16 i;
    for (i=0; str[i] != '\0'; ++i) {
        VID_MEM[ _term_cursor ] = (VID_MEM[ _term_cursor ] & 0xFF00) | str[i];
        _term_cursor++;
    }

    VID_MEM[ _term_cursor ] = '\n';
    _term_cursor++;
    */

    // --------------------------------------------------
    char *VID_MEM = (char *)0xb8000;
    uint16 i=0;

    while (str[i]) {
        VID_MEM[ _term_cursor ] = str[i];
        VID_MEM[ _term_cursor+1 ] = 0x2;

        _term_cursor += 2;
        i++;
    }

    VID_MEM[ _term_cursor ] = 0x10;
    VID_MEM[ _term_cursor+1 ] = 0x2;

    _term_cursor += 2;

    // --------------------------------------------------
}

// ====================================================================
void write_out(int nwords) {
    uint8 *screen = (uint8 *)(0xB8000 + 10);

    do {
        screen[0] = (nwords%10)+'0';
        screen[1] = 7; // Gray on black
        screen -=2;
        nwords /= 10;
    } while (nwords);
}

// ====================================================================
uint8 SCR_W = 80;
uint8 SCR_H = 25;

void clear_screen_buffer() {
    uint8 sym[] = " -.=a0";

    uint8 *VID_MEM = (uint8 *)0xb8000;

    while ( _term_cursor < (SCR_W*SCR_H) ) {
        VID_MEM[ _term_cursor ] = (VID_MEM[ _term_cursor ] & 0xFF00) | sym[0];
        _term_cursor++;
    }

    _term_cursor = 0;

}

// ====================================================================
//void kernel_main( void * MB_structure, uint8 MAGIC ) {
void kernel_main( void * MB_structure, uint32 MAGIC ) {

    clear_screen_buffer();

    uint8 A = 65;

    write_out( A );
    write_out( A );
    write_out( A );

    //_printf(" KERNEL: [TMX.OS]: [kernel.bin]\n");
    //_printf(" KERNEL: [TMX.OS]: [kernel.bin]\n");
    //_printf(" KERNEL: [TMX.OS]: [kernel.bin]\n");


    while (1);

}

// ====================================================================









