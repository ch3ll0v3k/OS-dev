#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "kernel_types.h"
#include "kernel_terminal.h"

void kernel_log( void * _str ) {

    // TODO: data logging
    //kernel_log("");
}

void kernel_log_info( void * _str ) {
    k_term_color = K_TERM_L_BLUE; k_term_print( "[info ]: " );
    k_term_color = K_TERM_WHITE;  k_term_printnl( "information  " );

}

void kernel_log_error( void * _str ) {
    k_term_color = K_TERM_L_RED; k_term_print( "[error]: " );
    k_term_color = K_TERM_WHITE; k_term_printnl( "information  " );

}

void kernel_log_std( void * _str ) {
    k_term_color = K_TERM_WHITE; k_term_print( "[ +++ ]: " );
    k_term_color = K_TERM_WHITE; k_term_printnl( "information  " );

}

void kernel_log_ok( void * _str ) {
    k_term_color = K_TERM_L_GREEN; k_term_print( "[ ok  ]: " );
    k_term_color = K_TERM_WHITE;    k_term_printnl( "information  " );

}

#endif
