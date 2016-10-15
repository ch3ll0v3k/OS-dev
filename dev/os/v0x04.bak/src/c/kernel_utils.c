#include "h/kernel_utils.h"

// ====================================================================
// TODO: larger integer needet
int8_t itoa(char *buff_norm, long  _long ) {

    char buff_rev[64];
    int8_t buff_c = 0;

    uint8_t is_positive = 1;

    if ( _long < 0 ) {
        is_positive = 0;
        uint32_t pos = 0;
        while ( _long < 0 ) {
            pos++;
            _long++;
        }

        _long += pos;

    }

    while ( _long > 9 ) {
        buff_rev[ buff_c++ ] = (_long % 10) + '0';
        _long /= 10;

    }


    buff_rev[ buff_c++ ] = _long+'0';
    if ( is_positive == 0 ) buff_rev[ buff_c++ ] = '-';

    int i = buff_c-1;
    buff_c = 0;


    while ( i > -1 )
        buff_norm[ buff_c++ ] = buff_rev[ i-- ];


    return 0;
}

// ====================================================================



