#include "h/kernel.h"

// ====================================================================
uint32 strlen( char * _str ) {

    uint32 _strlen = 0;

    while ( _str[ _strlen++ ] );

    return --_strlen;
}

// ====================================================================
uint32 strcmp( char * _a, char * _b ) {

    uint32 _a_len = strlen( _a );
    uint32 _b_len = strlen( _b );

    if ( _a_len > _b_len ) {
        return _a_len - _b_len;

    } else if ( _a_len < _b_len  ) {
        return _b_len - _a_len;

    } else {

        uint32 i=0;

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
uint8 printf( char* _str, ... ) {

    // TODO: _ret
    uint32 _ret = 0;

    uint32 _strlen = strlen( _str );

    char buff[512];
    uint32 buff_i = 0;

    va_list _va_list;
    va_start( _va_list, 0);

    char c[2]; c[1] = '\0';

    // -------------------------------------------------
    uint32 i = 0;
    while ( i < _strlen ) {

        if ( _str[ i ] == '%' && _str[ i+1 ] != '%' ) {
            if ( _str[ i+1 ] == 'd' ) {

                buff[buff_i] = '\0';
                buff_i = 0;
                k_term_print( buff );

                k_term_print_int( va_arg(_va_list, int) );
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

                c[0] = va_arg(_va_list, uint32)+'\0';

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
uint8 sprintf( char *_str_out, char *_str, ... ) {

    //k_term_printnl( _str );

    // TODO: _ret
    uint32 _ret = 0;

    uint32 _strlen = strlen( _str );

    char buff[512];
    uint32 buff_i = 0;
    uint32 buff_g_i = 0;
    //uint32 buff_len = 0;

    va_list _va_list;
    va_start( _va_list, 0);

    char int_char_buff[32];

    char *cc;

    // -------------------------------------------------
    uint32 i = 0;
    while ( i < _strlen ) {

        if ( _str[ i ] == '%' && _str[ i+1 ] != '%' ) {
            if ( _str[ i+1 ] == 'd' ) {

                buff[buff_i] = '\0';
                buff_i = 0;

                while ( buff[ buff_i ] != '\0' ) {
                    _str_out[buff_g_i] = buff[ buff_i ];
                    buff_g_i++; buff_i++;
                }

                _int2char( va_arg(_va_list, int), int_char_buff );
                int_char_buff[ strlen(int_char_buff)+1 ] = '\0';

                buff_i = 0;

                while ( int_char_buff[ buff_i ] != '\0' ) {
                    _str_out[buff_g_i] = int_char_buff[ buff_i ];
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

                _str_out[buff_g_i] = va_arg(_va_list, uint32);

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
void _int2char( int _int, char *_str_out ) {

    uint32 _str_out_i = 0;
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

    char buff[32];
    uint16 buff_c = 0;

    while ( _int > 9 ) {

        buff[ buff_c++ ] = (_int % 10) + '0';
        _int /= 10;

    }

    buff[ buff_c++ ] = _int+'0';
    buff[ buff_c ] = '\0';


    int i = buff_c;
    buff_c = 0;

    if ( is_positive == 0 ) {
        _str_out[ _str_out_i++ ] = '-';
    }

    for ( ; i>0; i-- ) {
        _str_out[ _str_out_i++ ] = buff[ i-1 ];

    }

    _str_out[ _str_out_i ] = '\0';

}

// ====================================================================


