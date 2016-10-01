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
// [PORTS]
uint8_t inb( uint16_t _port );
void outb( uint16_t _port, uint8_t _data );

uint16_t inw( uint16_t port );
void outw( uint16_t port, uint16_t data );

void outsw ( uint16_t port, const void *data, uint16_t size );
void insw ( uint16_t port, void *data, uint16_t size );

// ====================================================================
void _asm_nop();
void _asm_rep_nop();
void _asm_hlt();

void sleep( uint32_t sec );
void usleep( uint32_t u_sec );

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
void system_reboot();

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