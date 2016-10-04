#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// ====================================================================
typedef unsigned long long uint64_t;
typedef long long int64_t;

typedef unsigned int uint32_t;
typedef int int32_t;

typedef unsigned short uint16_t;
typedef short int16_t;

typedef unsigned char uint8_t;
//typedef char int8_t;

typedef unsigned long ulong_t;
typedef long long_t;

// ====================================================================
int main(int argc, char const *argv[]) {


    // if ( i_mode == 0x00 ) return 'U   0'; // ET2_FT_UNKNOWN   0   Unknown File Type
    // if ( i_mode == 0x01 ) return 'R   1'; // ET2_FT_REG_FILE  1   Regular File
    // if ( i_mode == 0x02 ) return 'D   2'; // ET2_FT_DIR       2   Directory File
    // if ( i_mode == 0x03 ) return 'C   3'; // ET2_FT_CHRDEV    3   Character Device
    // if ( i_mode == 0x04 ) return 'B   4'; // ET2_FT_BLKDEV    4   Block Device
    // if ( i_mode == 0x05 ) return 'F   5'; // ET2_FT_FIFO      5   Buffer File
    // if ( i_mode == 0x06 ) return 'S   6'; // ET2_FT_SOCK      6   Socket File
    // if ( i_mode == 0x07 ) return 'S   7'; // ET2_FT_SYMLINK   7   Symbolic Link


    uint8_t data = 0;

    data = ( 0 | 0  ); printf("[%d] [0x%x]\n", data, data );
    data = ( 0 | 10 ); printf("[%d] [0x%x]\n", data, data );
    data = ( 0 | 11 ); printf("[%d] [0x%x]\n", data, data );
    data = ( 0 | 12 ); printf("[%d] [0x%x]\n", data, data );
    data = ( 0 | 13 ); printf("[%d] [0x%x]\n", data, data );
    data = ( 0 | 14 ); printf("[%d] [0x%x]\n", data, data );
    data = ( 0 | 15 ); printf("[%d] [0x%x]\n", data, data );
    data = ( 0 | 16 ); printf("[%d] [0x%x]\n", data, data );




























    /*
        uint8_t data = 0;

        data = ( 1 << 0 )-1; printf("[%d] [0x%x]\n", data, data );
        data = ( 1 << 1 )-1; printf("[%d] [0x%x]\n", data, data );
        data = ( 1 << 2 )-1; printf("[%d] [0x%x]\n", data, data );
        data = ( 1 << 3 )-1; printf("[%d] [0x%x]\n", data, data );
        data = ( 1 << 4 )-1; printf("[%d] [0x%x]\n", data, data );
        data = ( 1 << 5 )-1; printf("[%d] [0x%x]\n", data, data );
        data = ( 1 << 6 )-1; printf("[%d] [0x%x]\n", data, data );
        data = ( 1 << 7 )-1; printf("[%d] [0x%x]\n", data, data );
        data = ( 1 << 7 )-1; printf("[%d] [0x%x]\n", data, data );
        data = ( 1 << 8 )-1; printf("[%d] [0x%x]\n", data, data );


    */

    return 0;

}

// ====================================================================



