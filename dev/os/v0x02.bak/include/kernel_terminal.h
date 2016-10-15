#ifndef _KERNEL_TERMINL_H_
#define _KERNEL_TERMINL_H_

#define TERM_W 80
#define TERM_H 25

#define TERM_WD TERM_W*2
#define TERM_HD TERM_H*2
#define TERM_MAX_BYTES (TERM_W * TERM_H * 2) -1

#define K_TERM_BLACK     0
#define K_TERM_BLUE      1
#define K_TERM_GREEN     2
#define K_TERM_CYAN      3
#define K_TERM_RED       4
#define K_TERM_MAGENTA   5
#define K_TERM_BROWN     6
#define K_TERM_L_GREY    7
#define K_TERM_D_GREY    8
#define K_TERM_L_BLUE    9
#define K_TERM_L_GREEN   10
#define K_TERM_L_CYAN    11
#define K_TERM_L_RED     12
#define K_TERM_L_MAGENTA 13
#define K_TERM_L_BROWN   14
#define K_TERM_WHITE     15

int k_term_color;

void k_term_clear();
void k_term_print( void* );
void k_term_println( void* );

void k_term_print_int( int _int );
void k_term_print_intnl( int _int );



#endif