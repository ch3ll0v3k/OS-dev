#include "h/kernel_system.h"

// ====================================================================
uint8 inportb( uint16 _port ) {

    uint8 r;
    __asm__ __volatile__ ( "inb %1, %0" : "=a" (r) : "dN" (_port) );
    return r;

}

void outportb( uint16 _port, uint8 _data ) {
    __asm__ __volatile__ ( "outb %1, %0" : : "dN" (_port), "a" (_data) );

}


/*
static inline void outb ( uint16 port, uint8 data ) {
    __asm__ __volatile__ ( "outb %b0, %w1" : : "a" (data), "d" (port) );
}

// Read from 8-bit port
// param port  Port number
// returns Read data
static inline uint8 inb ( uint16 port ) {
    uint8 r;
    __asm__ __volatile__ ( "inb %w1, %b0" : "=a" (r) : "d" (port) );
    return r;
}
*/

// Write to 16-bit port
// param port  Port number
// param data  Data to be sent
/*
static inline void outw ( uint16 port, uint16 data ) {
    __asm__ __volatile__ ( "outw %w0, %w1" : : "a" (data), "d" (port) );
}*/

// Read from 16-bit port
// param port  Port number
// returns Read data
/*
static inline uint16 inw ( uint16 port ) {
    uint16 r;

    __asm__ __volatile__ ( "inw %w1, %w0" : "=a" (r) : "d" (port) );
    return r;
}*/

// Write string to 16-bit port
// port: Port number
// data: String to be sent
// size: Size of Data
/*
static inline void outsw ( uint16 port, const void *data, uint16 size ) {
    __asm__ __volatile__ (
        "cld\n\t"
        "rep outsw\n\t"
        : "+S" (data), "+c" (size) : "d" (port)
    );
}*/

// Read string from 16-bit port
// port: Port number
// data: Address where to store read string
// size: Size of Data
/*
static inline void insw ( uint16 port, void *data, uint16 size ) {
    __asm__ __volatile__ (
        "cld\n\t"
        "rep insw\n\t"
        : "+D" (data), "+c" (size) : "d" (port)
    );
}*/

// ====================================================================
void _asm_nop() {       __asm__ __volatile__ ( "nop" : : ); }
void _asm_hlt() {       __asm__ __volatile__ ( "hlt" : : ); }
void _asm_pause() {     __asm__ __volatile__ ( "pause" : : ); } // SSE-2
void _asm_rep_nop() {   __asm__ __volatile__ ( "rep\n\tnop" : : ); }

// ====================================================================
void _sleep( uint32 sec ) {

    uint64 x = 0; uint64 i = 0;

    while ( x < sec*10000000*20 ) {

        i = 0;
        while ( i < 1000000000) i++;
        x++;
        //_asm_nop();
        __asm__ __volatile__ ( "nop" : : );
    }


}

void _usleep( uint32 u_sec ) {

    uint64 x = 0; uint64 i = 0;

    while ( x < u_sec*10000*20 ) {

        i = 0;
        while ( i < 1000000000) i++;
        x++;
        //_asm_nop();
        __asm__ __volatile__ ( "nop" : : );
    }


}

// ====================================================================
/*void kernel_log( void * _str ) {
    // TODO: data logging
    //kernel_log("");
}
*/

void kernel_log_info( void * _str ) {
    k_term_color = K_TERM_L_BLUE; k_term_print( "[info ]: " );
    k_term_color = K_TERM_WHITE;  k_term_printnl( _str );
    _usleep(100);

}

void kernel_log_error( void * _str ) {
    k_term_color = K_TERM_L_RED; k_term_print( "[error]: " );
    k_term_color = K_TERM_WHITE; k_term_printnl( _str );
    _usleep(1000);

}

void kernel_log_std( void * _str ) {
    k_term_color = K_TERM_WHITE; k_term_print( "[ +++ ]: " );
    k_term_color = K_TERM_WHITE; k_term_printnl( _str );
    _usleep(100);

}

void kernel_log_ok( void * _str ) {
    k_term_color = K_TERM_L_GREEN; k_term_print( "[ ok  ]: " );
    k_term_color = K_TERM_WHITE;   k_term_printnl( _str );
    _usleep(100);

}

// ====================================================================
void system_halt() {

    k_term_clear();

    kernel_log_info(" SYSTEM: HALT: in [3]\n");

    _sleep(1);
    k_term_print("3 "); _sleep(1);
    k_term_print("2 "); _sleep(1);
    k_term_print("1 "); _sleep(1);

    k_term_print("\npower off now \n");
    _asm_hlt();

}

// ====================================================================

/*The following array is taken from
http://www.osdever.net/bkerndev/Docs/keyboard.htm
All credits where due*/

unsigned char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b', /* Backspace */
    '\t',         /* Tab */
    'q', 'w', 'e', 'r',   /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`',   0,        /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
    'm', ',', '.', '/',   0,              /* Right shift */
    '*',
    0,  /* Alt */
    ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};
