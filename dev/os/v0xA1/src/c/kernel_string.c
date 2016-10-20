#include "h/kernel.h"

// ====================================================================
/*
uint8_t  FFF(WATCH) {

    // -------------------------------------------------
    char chr[10] = "0123456789";
    char chr2[9];
    char* ch = chr;
    char* ch2 = chr2;
    uint8_t integ = 0;

    do {
        ch2[integ] = ch[value%10];

        printch(ch2[integ]);
        integ++;

    } while ((value /= 10) > 0);
    // -------------------------------------------------
    char chr[16] = "0123456789abcdef";

    uint32_t X = 10240;
    uint32_t i = 0;
    uint16_t _base_ = 10; // only with base of 10

    printf( "---------------\n" );
    while ( X > 0 ) {
        i = X;
        X /= 2;
        printf( "[%d] == ", i );
        do {
            printf( "%d | %c, ", i % _base_, chr[ i % _base_ ] );
        } while ( (i /= _base_ ) > 0 );
        printf( "\n" );

    }

    // -------------------------------------------------
}
*/

// ====================================================================
uint8_t cupper (char c) {

    // uint8_t A = 65+'\0';
    // uint8_t a = A+32+'\0';

    if ( c >= 65+32 && c <= 65+32+26 )
        return c-32+'\0';

    return c;

}

uint8_t clower (char c) {

    if ( c >= 65 && c <= 65+26 )
        return c+32+'\0';
    return c;

}

uint8_t strupper (char *c) {

    uint32_t len, i;

    len = strlen(c);
    i = 0;

    while ( i < len) {
        c[i] = cupper( c[i] ); i++;
    }

    return 0;
}


uint8_t strlower (char *c) {

    uint32_t len, i;

    len = strlen(c);
    i = 0;

    while ( i < len) {
        c[i] = clower( c[i] ); i++;
    }

    return 0;
}


// ====================================================================
void temp() {

    /*
    char HEX_CHAR[] = "0123456789abcdef";
    char HEX[32];


    uint32_t _res = 1234567890;


    uint16_t AX,i;
    AX = i = 0;

    for ( i = 0; i < 16; i++) {
        //HEX[ AX ] = HEX_CHAR[ _res[i] >> 4  ]; AX++;
        //HEX[ AX ] = HEX_CHAR[ _res[i] & 0xF ]; AX++;
    }

    HEX[32] = '\0';
    */
}

// ====================================================================
uint8_t isalpha( char c ) { return c; }
uint8_t isdigit( char c ) { return c; }

// ====================================================================
void hex2dec(char _a, char _b ) {

    int db_index = 0;
    db_index +=  isalpha( _a ) ? cupper(_a) - 'A' + 10 : _a - '0';
    db_index +=  (isalpha( _b ) ? cupper(_b) - 'A' + 10 : _b - '0') * 16;

}



// ====================================================================
// int8_t itox(char *buff_norm, long  _long ) {}

// ====================================================================
// TODO: larger integer needet
int8_t itoa(char *buff_norm, int64_t num, uint8_t base ) {

    uint8_t buff_rev[64];
    int16_t buff_c = 0;
    int16_t i = 0;
    int32_t rest;
    uint8_t is_positive = 1;

    switch (base) {

        // [BIN]
        case 2:     break;

        // [8--]
        case 8:     break;

        // [DEC]
        case 10:

            // ----------------------------------------
            if ( num == 0 ) {
                buff_norm[0] = '0';
                buff_norm[1] = '\0';
                break;

            }

            if ( num < 0 ) {
                is_positive = 0;
                uint32_t pos = 0;
                while ( num < 0 ) {
                    pos++;
                    num++;
                }

                num += pos;

            }

            while ( num > 9 ) {
                buff_rev[ buff_c++ ] = (num % 10) + '0';
                num /= 10;

            }


            buff_rev[ buff_c++ ] = num+'0';
            if ( is_positive == 0 )
                buff_rev[ buff_c++ ] = '-';

            i = buff_c-1;
            buff_c = 0;


            while ( i > -1 )
                buff_norm[ buff_c++ ] = buff_rev[ i-- ];

            buff_norm[ buff_c ] = '\0';

            break;

        // [HEX]
        case 16:

            // ----------------------------------------
            if ( num == 0 ) {
                buff_norm[0] = '0';
                buff_norm[1] = '\0';
                break;

            }

            //rest = num % 16;
            while ( num  ) {

                //k_term_print_intnl(num);
                rest = num % 16;
                num /= 16;

                // upper / lower
                if ( rest+55 < 65 ) buff_rev[ buff_c++] = (char)(rest+48);
                else buff_rev[ buff_c++] = (char)(rest+55);

            }

            buff_c--;

            while ( buff_c > -1 )
                buff_norm[ i++ ] = (char)buff_rev[ buff_c-- ];

            buff_norm[ i ] = '\0';

            break;
    }

    return 0;

}

// ====================================================================
uint32_t strlen( char * _str ) {

    uint32_t _strlen = 0;

    while ( _str[ _strlen++ ] );

    return --_strlen;
}

// ====================================================================
uint32_t strcmp( char * _a, char * _b ) {

    uint32_t _a_len = strlen( _a );
    uint32_t _b_len = strlen( _b );

    if ( _a_len > _b_len ) {
        return _a_len - _b_len;

    } else if ( _a_len < _b_len  ) {
        return _b_len - _a_len;

    } else {

        uint32_t i=0;

        while ( i < _a_len ) {

            if ( _a[ i ] > _b[ i ] ) return _a[ i ] - _b[ i ];
            else if ( _a[ i ] < _b[ i ] ) return _b[ i ] - _a[ i ];
            else {
                i++;
            }
        }

    }

    return 0;
}

// ====================================================================
uint8_t strcpy( /*TODO:*/ ) {

    return 0;

}

// ====================================================================
uint8_t printf( char* _str, ... ) {

    // FIXME:
    // FIXME:
    // FIXME: uint32_t is MAX >> need [SIGNED] 64 bit
    // FIXME:
    // FIXME:

    uint32_t _ret = 1; // TODO: _ret

    uint32_t _strlen = strlen( _str );

    char buff[512];
    uint32_t buff_i = 0;

    va_list _va_list;
    va_start( _va_list, 0);

    char c[2] = { '0', '\0' };

    // -------------------------------------------------
    char itoa_buff[96];
    // -------------------------------------------------
    uint32_t i = 0;

_while:

    while ( i < _strlen ) {

        if ( _str[ i ] == '%' && _str[ i+1 ] != '%' ) {

            switch ( _str[i+1] ) {

                case 'd': // break;
                case 'u':
                    buff[buff_i] = '\0'; buff_i = 0;

                    k_term_print( buff );
                    itoa( itoa_buff, va_arg(_va_list, uint32_t), 10 ); // FIXME: 32 bit
                    k_term_print( itoa_buff );

                    i += 2;
                    goto _while;

                case 'x': // break;
                case 'X': // break;

                    buff[buff_i] = '\0'; buff_i = 0;

                    k_term_print( buff );
                    itoa( itoa_buff, va_arg(_va_list, uint32_t), 16 ); // FIXME: 32 bit
                    if ( _str[i+1] == 'x' ) strlower( itoa_buff );
                    k_term_print( itoa_buff );

                    i += 2;
                    goto _while;

                case 's':
                case 'c':

                    buff[buff_i] = '\0'; buff_i = 0;
                    k_term_print( buff );

                    if ( _str[i+1] == 's' ) {
                        k_term_print( va_arg(_va_list, char*) );

                    } else {
                        c[0] = va_arg(_va_list, uint32_t)+'\0';
                        k_term_print( c );

                    }

                    i += 2;
                    goto _while;

                default:
                    break;


            }


        } else {

            if ( _str[i] == '\t' ) {
                for (int i = 0; i < 4; i++) buff[buff_i++] = ' ';


            } else {
                buff[buff_i++] = _str[i];

            }


        }

        i++;

    }

    buff[buff_i] = '\0';

    k_term_print( buff );
    va_end(_va_list);

    return _ret;

}


// ====================================================================
uint8_t sprintf( char *_str_out, char *_str, ... ) {

    uint32_t _ret = 1; // TODO: _ret
    uint32_t _strlen = strlen( _str );

    char buff[512];
    uint32_t buff_i = 0;
    uint32_t buff_g_i = 0;
    //uint32_t buff_len = 0;

    va_list _va_list;
    va_start( _va_list, 0);

    char *cc;
    char itoa_buff[50];

    uint32_t i = 0;
    // -------------------------------------------------

_while:
    while ( i < _strlen ) {

        if ( _str[ i ] == '%' && _str[ i+1 ] != '%' ) {

            switch (_str[i+1] ) {

                case 'd': // break;
                case 'u': // break;

                    buff[buff_i] = '\0'; buff_i = 0;

                    while ( buff[ buff_i ] != '\0' )
                        _str_out[ buff_g_i++ ] = buff[ buff_i++ ];

                    itoa( itoa_buff, va_arg(_va_list, uint32_t), 10 ); // FIXME: 32 bit

                    itoa_buff[ strlen(itoa_buff)+1 ] = '\0';

                    buff_i = 0;

                    while ( itoa_buff[ buff_i ] != '\0' )
                        _str_out[ buff_g_i++ ] = itoa_buff[ buff_i++ ];

                    buff_i = 0; i += 2;
                    goto _while;

                case 'x': // break;
                case 'X': // break;

                    buff[buff_i] = '\0'; buff_i = 0;

                    while ( buff[ buff_i ] != '\0' )
                        _str_out[ buff_g_i++ ] = buff[ buff_i++ ];

                    itoa( itoa_buff, va_arg(_va_list, uint32_t), 16 ); // FIXME: 32 bit

                    itoa_buff[ strlen(itoa_buff)+1 ] = '\0';

                    if ( _str[i+1] == 'x' ) strlower( itoa_buff );

                    buff_i = 0;

                    while ( itoa_buff[ buff_i ] != '\0' )
                        _str_out[ buff_g_i++ ] = itoa_buff[ buff_i++ ];

                    buff_i = 0; i += 2;

                    goto _while;


                case 's':

                    buff[buff_i] = '\0'; buff_i = 0;

                    while ( buff[ buff_i ] != '\0' )
                        _str_out[ buff_g_i++ ] = buff[ buff_i++ ];

                    cc = va_arg(_va_list, char *)+'\0';
                    buff_i = 0;

                    while ( cc[ buff_i ] != '\0' )
                        _str_out[ buff_g_i++ ] = cc[ buff_i++ ];

                    buff_i = 0; i += 2;

                    goto _while;

                case 'c':

                    buff[buff_i] = '\0'; buff_i = 0;

                    while ( buff[buff_i] != '\0' )
                        _str_out[buff_g_i++] = buff[ buff_i++ ];

                    _str_out[ buff_g_i++ ] = (char)(va_arg(_va_list, int32_t));

                    buff_i = 0; i += 2;

                    goto _while;

                default:
                    break;

            }



        } else {

            if ( _str[i] == '\t' ) {
                for (int i = 0; i < 4; i++) buff[buff_i++] = ' ';

            } else {
                buff[buff_i++] = _str[i];

            }

        }

        i++;

    }

    buff[buff_i] = '\0';
    buff_i = 0;

    while ( buff[ buff_i ] != '\0' ) {
        _str_out[buff_g_i] = buff[ buff_i ];
        buff_g_i++; buff_i++;
    }

    _str_out[buff_g_i] = '\0';

    va_end(_va_list);

    return _ret;

}



// ====================================================================
/*
void printf (const char *format, ...) {
    char **arg = (char **) &format;
    int c;
    char buf[20];

    arg++;

    while ((c = *format++) != 0) {
        if (c != '%')
            putchar (c);
        else {
            char *p;

            c = *format++;
            switch (c) {
                case 'd':
                case 'u':
                case 'x':
                    itoa (buf, c, *((int *) arg++));
                    p = buf;
                    goto string;
                    break;

                case 's':
                    p = *arg++;
                    if (! p)
                        p = "(null)";

                string:
                    while (*p)
                        putchar (*p++);
                    break;

                default:
                    putchar (*((int *) arg++));
                    break;
            }
        }
    }
}

*/