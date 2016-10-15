#ifndef _KERNEL_SYSTEM_H_
#define _KERNEL_SYSTEM_H_

#include "kernel.h"

// ====================================================================
// https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html

/*
+------------------------------+------------------------------------+
|       Intel Code             |      AT&T Code                     |
+------------------------------+------------------------------------+
| mov     eax,1                |  movl    $1,%eax                   |
| mov     ebx,0ffh             |  movl    $0xff,%ebx                |
| int     80h                  |  int     $0x80                     |
| mov     ebx, eax             |  movl    %eax, %ebx                |
| mov     eax,[ecx]            |  movl    (%ecx),%eax               |
| mov     eax,[ebx+3]          |  movl    3(%ebx),%eax              |
| mov     eax,[ebx+20h]        |  movl    0x20(%ebx),%eax           |
| add     eax,[ebx+ecx*2h]     |  addl    (%ebx,%ecx,0x2),%eax      |
| lea     eax,[ebx+ecx]        |  leal    (%ebx,%ecx),%eax          |
| sub     eax,[ebx+ecx*4h-20h] |  subl    -0x20(%ebx,%ecx,0x4),%eax |
+------------------------------+------------------------------------+*/

// ====================================================================
uint8 inportb( uint16 _port );
void outportb( uint16 _port, uint8 _data );

// Read from 8-bit port
// param port  Port number
// returns Read data
inline uint8 inb ( uint16 port ) {
    uint8 r;
    __asm__ __volatile__ ( "inb %w1, %b0" : "=a" (r) : "d" (port) );
    return r;
}

inline void outb ( uint16 port, uint8 data ) {
    __asm__ __volatile__ ( "outb %b0, %w1" : : "a" (data), "d" (port) );
}


void _asm_nop();
void _asm_rep_nop();
void _asm_hlt();


void _sleep( uint32 sec );
void _usleep( uint32 u_sec );


// ====================================================================
/*void kernel_log( void * _str ) {
    // TODO: data logging
    //kernel_log("");
}*/

void kernel_log_info( void * _str );
void kernel_log_error( void * _str );
void kernel_log_std( void * _str );
void kernel_log_ok( void * _str );

// ====================================================================
void system_halt();


// ====================================================================
/*
#define sleep_mode()                           \
    do {                                           \
        _SLEEP_CONTROL_REG |= _BV(SE);             \
        __asm__ __volatile__ ("sleep" "\n\t" :: ); \
        _SLEEP_CONTROL_REG &= ~_BV(SE);            \
    } while (0)

#define sleep_cpu()                              \
    do {                                             \
      __asm__ __volatile__ ( "sleep" "\n\t" :: );    \
    } while(0)
*/

// ====================================================================

#endif