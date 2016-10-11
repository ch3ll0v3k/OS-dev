#ifndef _KERNEL_STRING_H_
#define _KERNEL_STRING_H_

#include "kernel.h"

// ====================================================================
// TODO: larger integer needet [long _long]
int8_t itoa(char *buff_norm, long  _long, uint8_t base );

int8_t itox(char *buff_norm, long  _long );

// TODO : int32 atoi();
uint8_t strcpy( /*TODO:*/ );


uint8_t cupper (char c);
uint8_t clower (char c);

uint8_t strupper (char *c);
uint8_t strlower (char *c);

uint32_t strlen( char *_str );
uint32_t strcmp( char *_a, char * _b );

uint8_t printf( char *_str, ... );
uint8_t sprintf( char* _str_out, char* _str, ... );

// ====================================================================

#endif