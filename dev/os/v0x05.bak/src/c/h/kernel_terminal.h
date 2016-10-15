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

/*

0x60 = black on orange
*/

// ====================================================================
void k_term_status_bar_draw();

char k_term_status_bar_buff[ TERM_W ];
int16_t k_term_status_bar_color; // 0x04;


// ---------------------------
uint32_t TERM_TICKS_AREA;
int k_term_color;

char k_term_cmd_buff[512];
char *k_term_cmds[256];
uint32_t *k_term_func[256];

int32_t k_term_cmd_c;
int32_t k_term_cmd_ttl;

char k_term_history_map[256][256];
uint8_t k_term_history_items;
uint8_t k_term_history_curr_item;

// ---------------------------

void k_term_init();
void reset_promt();
void k_term_process_input( REG *r );

void k_term_clear_line( uint8_t line );
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













