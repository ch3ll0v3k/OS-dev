#ifndef _KERNEL_STRING_H_
#define _KERNEL_STRING_H_

#include "kernel.h"

// ====================================================================
void _int2char( int _int, char *_str_out );

uint32 strlen( char *_str );
uint32 strcmp( char *_a, char * _b );

uint8 printf( char *_str, ... );
uint8 sprintf( char* _str_out, char* _str, ... );
//uint8 printf( uint16 argc, ... );



// ====================================================================

#endif