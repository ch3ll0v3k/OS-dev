#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// ====================================================================
typedef unsigned long long uint64_t;
typedef long long int64_t;

// ** typedef unsigned long int uint64_t;
// ** typedef long int int64_t;

typedef unsigned int uint32_t;
typedef int int32_t;

typedef unsigned short uint16_t;
typedef short int16_t;

typedef unsigned char uint8_t;
//typedef char int8_t;

typedef unsigned long ulong_t;
typedef long long_t;

// ====================================================================
// 1 hour   3600 seconds
// 1 day    86400 seconds
// 1 week   604800 seconds
// 1 month  (30.44 days)      2629743 seconds
// 1 year   (365.24 days)     31556926 seconds
// ====================================================================
void get_time( uint64_t tt) {


    //uint64_t UNIX = 1475375435; // seconds since Jan 01 1970. (UTC); // Вторник
    uint64_t UNIX = tt; // seconds since Jan 01 1970. (UTC); // Вторник
    uint32_t C = 0;

    uint32_t s_y = 1970;
    uint32_t s_m = 1;
    uint32_t s_d = 1;

    uint32_t s_wd = 3; // Вторник

    uint32_t s_h = 0;
    uint32_t s_i = 0;
    uint32_t s_s = 0;

    uint8_t MM_D[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    char *MM_S[12] = {
        "January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"
    };

    char *W_D[8] = { "[1] Ma", "[2] Di", "[3] Wo", "[4] Do", "[5] Vr", "[6] Za", "[7] Zo" };


    uint32_t LOCAL_TIME_DIFF = 7200;

    UNIX += ( LOCAL_TIME_DIFF );


    while ( UNIX > 31449600 ) {

        s_y++;
        if ( s_y % 4 == 0 ) {
            UNIX -= 31536000;

        } else {
            UNIX -= 31449600;
        }


    }

    UNIX -= ((86400) * 45);



    uint32_t DAYS_IN_YEAR = 1;
    uint64_t SEC_IN_YEAR = 0;

    while ( C < UNIX ) {
        //  J   F   M   A   M   J   J   A   S   O   N   D
        //  1   2   3   4   5   6   7   8   9   10  11  12
        //  31 [2x] 31  30  31  30  31  31  30  31  30  31


        if ( s_s > 59 ) { // SEC
            s_s = 0;  s_i++;

            if ( s_i > 59 ) { // MIN
                s_i = 0;  s_h++;

                if ( s_h > 23 ) { // HOUR

                    s_h = 0;  s_d++;

                    s_wd++; if ( s_wd > 6 ) s_wd = 0;
                    //DAYS_IN_YEAR++;
                    // -------------------------------------------
                    // DAYS
                    //printf("           | %s %d-%d-%d  %s \n", W_D[ s_wd ], s_y, s_m, s_d, MM_S[ s_m-1 ] );

                    if ( s_m == 2  ) { // is January

                        if ( s_y % 4 == 0 ) { // Is [HIGH YEAR]

                            if ( s_d > 28 ) {

                                //printf(" HIGH_YEAR | %s %d-%d-%d  %s \n", W_D[ s_wd ], s_y, s_m, s_d, MM_S[ s_m-1 ] );
                                s_d = 0; s_m++;

                            }

                        } else if ( s_d > 27 ) {
                            //printf(" **** **** | %s %d-%d-%d  %s \n", W_D[ s_wd ], s_y, s_m, s_d, MM_S[ s_m-1 ] );
                            s_d = 0; s_m++;

                        }

                    } else if ( s_d > (MM_D[s_m-1]-1) ) {

                        //printf(" --- \n" );
                        s_d = 0; s_m++;


                    }

                    // MONTH
                    if ( s_m > 12 ) {
                        /*if ( DAYS_IN_YEAR == 366 )
                            printf("Years in [%d] == [%d] [%d] ***\n", s_y, DAYS_IN_YEAR, SEC_IN_YEAR);
                        else
                            printf("Years in [%d] == [%d] [%d]\n", s_y, DAYS_IN_YEAR, SEC_IN_YEAR);
                        */

                        //DAYS_IN_YEAR = 0;
                        //SEC_IN_YEAR = 0;
                        s_m = 1; s_y++;
                    }



                } // HOUR
            } // MIN
        } // SEC


        //printf("[%d]| %s %d-%d-%d  %s \n", C, W_D[ s_wd ], s_y, s_m, s_d, MM_S[ s_m-1 ] );
        //usleep(10);

        s_s++;
        //SEC_IN_YEAR++;
        C++;

    }

    printf("--------------------------------------------------\n" );
    printf("%s %d-%d-%d %s %d:%d:%d \n", W_D[ s_wd ], s_y, s_m, s_d, MM_S[ s_m-1 ], s_h, s_i, s_s  );


}

// HIGH_YEAR | 1972-2-29 / 0:0:0
// HIGH_YEAR | 1976-2-29 / 0:0:0
// HIGH_YEAR | 1980-2-29 / 0:0:0
// HIGH_YEAR | 1984-2-29 / 0:0:0
// HIGH_YEAR | 1988-2-29 / 0:0:0
// HIGH_YEAR | 1992-2-29 / 0:0:0

// HIGH_YEAR | 1996-2-29 / 0:0:0
// HIGH_YEAR | 2000-2-29 / 0:0:0
// HIGH_YEAR | 2004-2-29 / 0:0:0
// HIGH_YEAR | 2008-2-29 / 0:0:0
// HIGH_YEAR | 2012-2-29 / 0:0:0
// HIGH_YEAR | 2016-2-29 / 0:0:0

// ====================================================================
int main(int argc, char const *argv[]) {


    uint64_t tt = 1475375435 - 100000;

    for (int i = 0; i < 100000; i+= 10000) {


        get_time( tt + i );

    }


    return 0;

}

// ====================================================================



