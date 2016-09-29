#include "h/kernel.h"

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
uint8_t printf( char* _str, ... ) {

    // TODO: _ret
    uint32_t _ret = 0;

    uint32_t _strlen = strlen( _str );

    char buff[512];
    uint32_t buff_i = 0;

    va_list _va_list;
    va_start( _va_list, 0);

    char c[2]; c[1] = '\0';

    // -------------------------------------------------
    char itoa_buff[72];
    // -------------------------------------------------
    uint32_t i = 0;
    while ( i < _strlen ) {

        if ( _str[ i ] == '%' && _str[ i+1 ] != '%' ) {
            if ( _str[ i+1 ] == 'd' ) {

                buff[buff_i] = '\0';
                buff_i = 0;
                k_term_print( buff );


                itoa( itoa_buff, va_arg(_va_list, int) );
                k_term_print( itoa_buff );

                i += 2;
                continue;


            } else if ( _str[ i+1 ] == 's' ) {

                buff[buff_i] = '\0';
                buff_i = 0;
                k_term_print( buff );

                k_term_print( va_arg(_va_list, char*) );
                i += 2;
                continue;

            } else if ( _str[ i+1 ] == 'c' ) {

                buff[buff_i] = '\0';
                buff_i = 0;
                k_term_print( buff );

                c[0] = va_arg(_va_list, uint32_t)+'\0';

                k_term_print( c );
                i += 2;
                continue;

            }


        } else {
            buff[buff_i] = _str[i];
            buff_i++;
        }

        i++;

    }

    buff[buff_i] = '\0';
    buff_i = 0;
    k_term_print( buff );
    va_end(_va_list);

    return _ret;

}


// ====================================================================
uint8_t sprintf( char *_str_out, char *_str, ... ) {

    //k_term_printnl( _str );

    // TODO: _ret
    uint32_t _ret = 0;

    uint32_t _strlen = strlen( _str );

    char buff[512];
    uint32_t buff_i = 0;
    uint32_t buff_g_i = 0;
    //uint32_t buff_len = 0;

    va_list _va_list;
    va_start( _va_list, 0);

    char *cc;
    char itoa_buff[50];

    // -------------------------------------------------
    uint32_t i = 0;
    while ( i < _strlen ) {

        if ( _str[ i ] == '%' && _str[ i+1 ] != '%' ) {
            if ( _str[ i+1 ] == 'd' ) {

                buff[buff_i] = '\0';
                buff_i = 0;

                while ( buff[ buff_i ] != '\0' ) {
                    _str_out[buff_g_i] = buff[ buff_i ];
                    buff_g_i++; buff_i++;
                }

                itoa( itoa_buff, va_arg(_va_list, int) );

                itoa_buff[ strlen(itoa_buff)+1 ] = '\0';

                buff_i = 0;

                while ( itoa_buff[ buff_i ] != '\0' ) {
                    _str_out[buff_g_i] = itoa_buff[ buff_i ];
                    buff_g_i++; buff_i++;
                }

                buff_i = 0;
                i += 2;
                continue;


            } else if ( _str[ i+1 ] == 's' ) {

                buff[buff_i] = '\0';
                buff_i = 0;

                while ( buff[ buff_i ] != '\0' ) {
                    _str_out[buff_g_i] = buff[ buff_i ];
                    buff_g_i++; buff_i++;
                }

                cc = va_arg(_va_list, char *)+'\0';
                buff_i = 0;

                while ( cc[ buff_i ] != '\0' ) {
                    _str_out[buff_g_i] = cc[ buff_i ];
                    buff_g_i++; buff_i++;

                }

                buff_i = 0;

                i += 2;
                continue;

            } else if ( _str[ i+1 ] == 'c' ) {

                buff[buff_i] = '\0';
                buff_i = 0;

                while ( buff[buff_i] != '\0' ) {
                    _str_out[buff_g_i] = buff[ buff_i ];
                    buff_g_i++; buff_i++;
                }

                _str_out[buff_g_i] = va_arg(_va_list, uint32_t);

                buff_i = 0;
                i += 2;
                continue;

            }


        } else {
            buff[buff_i] = _str[i];
            buff_i++;
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


