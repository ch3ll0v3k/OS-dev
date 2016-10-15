#include "include/kernel_types.h"
#include "include/kernel_terminal.h"

uint32 k_term_x_pos = 2;
uint32 k_term_y_pos = 0;

char *k_term_main_buff[ TERM_MAX_BYTES * 10 ];

k_term_color = K_TERM_WHITE;

void k_term_clear() {

    char * VID_BUFF = (char *) 0xb8000;

    int c=0;
    while ( c < (TERM_W * TERM_H * 2) ) {
        VID_BUFF[ c ] = 0;
        c += 2;

    }


}

// ====================================================================
void k_term_scroll_up() { }
void k_term_scroll_down() { }


// ====================================================================
void k_term_printnl( char * _str ) {

    k_term_print( _str );
    k_term_print( " \n" );

}

void k_term_print( void *_str) {

    char *str = (char *) _str;
    uint8 str_c = 0;

    char * VID_BUFF = (char *) 0xb8000;

    while ( str[ str_c ] ) {

        VID_BUFF[ k_term_y_pos + k_term_x_pos ] = str[ str_c ];
        VID_BUFF[ k_term_y_pos + k_term_x_pos +1 ] = k_term_color;

        if ( str[ str_c ] == '\n' || k_term_x_pos >= TERM_WD-2 ) {

            if ( str[ str_c ] == '\n' ) {
                VID_BUFF[ k_term_y_pos + k_term_x_pos ] = '\0';
            }

            k_term_y_pos += TERM_WD;
            k_term_x_pos = 2;

            /*
            if ( k_term_y_pos + k_term_x_pos >= TERM_MAX_BYTES ) {
                k_term_y_pos = 0;
                k_term_x_pos = 2;
                k_term_clear();
                k_term_print("FULL-TERM: >= TERM_MAX_BYTES ");
                return;

            }
            */

        } else {
            k_term_x_pos += 2;

        }

        str_c++;

    }

    /*
    if ( k_term_y_pos + k_term_x_pos >= TERM_MAX_BYTES ) {
        k_term_y_pos = 0;
        k_term_x_pos = 2;
        k_term_clear();
        k_term_print("FULL-TERM: >= TERM_MAX_BYTES ");
        return;
    }
    */


}

// ====================================================================
void k_term_print_intnl( int _int ) {

    k_term_print_int( _int );
    k_term_print( " \n" );

}

void k_term_print_int( int _int ) {

    char *VID_BUFF = (char *) 0xb8000;
    uint8 is_positive = 1;

    if ( _int < 0 ) {
        is_positive = 0;
        int pos = 0;
        while ( _int < 0 ) {
            pos++;
            _int++;
        }

        _int += pos;

    }

    char k_term_int_buff[32];
    uint16 k_term_int_buff_c = 0;

    while ( _int > 9 ) {

        k_term_int_buff[ k_term_int_buff_c++ ] = (_int % 10) + '0';
        _int /= 10;

    }

    k_term_int_buff[ k_term_int_buff_c++ ] = _int+'0';
    k_term_int_buff[ k_term_int_buff_c ] = '\0';



    int i = k_term_int_buff_c;
    k_term_int_buff_c = 0;

    if ( is_positive == 0 ) {
        VID_BUFF[ k_term_y_pos + k_term_x_pos ] = '-'; k_term_int_buff_c += 2;
        k_term_x_pos += 2;
    }

    for ( ; i>0; i-- ) {
        VID_BUFF[ k_term_y_pos + k_term_x_pos ] = k_term_int_buff[ i-1 ];
        k_term_x_pos += 2;

    }


}


// ====================================================================
// ====================================================================


