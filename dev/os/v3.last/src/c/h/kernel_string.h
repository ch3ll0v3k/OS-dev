#ifndef _KERNEL_STRING_H_
#define _KERNEL_STRING_H_

#include "kernel.h"

// ====================================================================
void _int2char( int _int, char *_str_out );

uint32_t strlen( char *_str );
uint32_t strcmp( char *_a, char * _b );

uint8_t printf( char *_str, ... );
uint8_t sprintf( char* _str_out, char* _str, ... );
//uint8_t printf( uint16 argc, ... );



// ====================================================================

#endif