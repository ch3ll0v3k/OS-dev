#ifndef _KERNEL_TERMINL_H_
#define _KERNEL_TERMINL_H_

#include "kernel.h"
// ====================================================================

#define TERM_W 80
#define TERM_H 25

#define TERM_WD TERM_W*2
#define TERM_HD TERM_H*2
#define TERM_MAX_BYTES ((TERM_W-2) * TERM_H * 2) -1

#define K_TERM_BLACK     0x0
#define K_TERM_BLUE      0x1
#define K_TERM_GREEN     0x2
#define K_TERM_CYAN      0x3
#define K_TERM_RED       0x4
#define K_TERM_MAGENTA   0x5
#define K_TERM_BROWN     0x6
#define K_TERM_L_GREY    0x7
#define K_TERM_D_GREY    0x8
#define K_TERM_L_BLUE    0x9
#define K_TERM_L_GREEN   0xa
#define K_TERM_L_CYAN    0xb
#define K_TERM_L_RED     0xc
#define K_TERM_L_MAGENTA 0xd
#define K_TERM_L_BROWN   0xe
#define K_TERM_WHITE     0xf



int k_term_color;

void k_term_init();

void k_term_get_input( char *_out );

void k_term_clear_line( uint8 line );
void k_term_scroll_up();
void k_term_check_height();

void k_term_update_cursor();
void k_term_clear();
void k_term_print( void* );
void k_term_printnl( void* );

void k_term_print_int( int _int );
void k_term_print_intnl( int _int );

void kernel_print_info();
void test_print_func();
void _asm_print();



// ====================================================================
#endif













