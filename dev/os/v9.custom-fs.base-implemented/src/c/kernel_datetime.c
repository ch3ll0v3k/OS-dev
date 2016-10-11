#include "h/kernel.h"
// kernel_datetime.c
// ====================================================================
// 1 hour   3600 seconds
// 1 day    86400 seconds
// 1 week   604800 seconds
// 1 month  (30.44 days)      2629743 seconds
// 1 year   (365.24 days)     31556926 seconds
// ====================================================================
void date_ISO_8601( uint64_t tt) {


    uint64_t UNIX = tt; // seconds since Jan 01 1970. (UTC); // Tue
    uint32_t C = 0;

    int32_t s_y = 1970;
    int32_t s_m = 1;
    int32_t s_d = 1;

    uint32_t s_wd = 3; // Вторник

    int32_t s_h = 0;
    int32_t s_i = 0;
    int32_t s_s = 0;

    int8_t MM_D[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    /*
    char *MM_S[12] = {
        "January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"
    };
    */

    //char *W_D[8] = { "[1] Ma", "[2] Di", "[3] Wo", "[4] Do", "[5] Vr", "[6] Za", "[7] Zo" };

    uint32_t _28 = 2419200; // sec in N days
    uint32_t _29 = 2505600; // sec in N days
    uint32_t _30 = 2592000; // sec in N days
    uint32_t _31 = 2678400; // sec in N days

    uint32_t H_Y = 31536000; // sec in ['HIGH YEAR'] February 29 days
    //uint32_t L_Y = 31449600; // sec in ['LOW YEAR'] February 28 days

    // ---------------------------------------------------------------
    // BIG JUMP // A year at the time
    while ( UNIX > H_Y ) {


        UNIX -= _31; // January

        if ( s_y % 4 == 0 )
            UNIX -= _29; // HIGH_YEAR -> February
        else
            UNIX -= _28; // LOW_YEAR -> February

        UNIX -= _31; // March
        UNIX -= _30; // April
        UNIX -= _31; // May
        UNIX -= _30; // June
        UNIX -= _31; // July
        UNIX -= _31; // August
        UNIX -= _30; // September
        UNIX -= _31; // October
        UNIX -= _30; // November
        UNIX -= _31; // December

        s_y++;
    }

    // ---------------------------------------------------------------
    // FINE JUMP // 1 month at the time

    if ( UNIX > _31 ) { // January
        UNIX -= _31; s_m++;

        if ( s_y % 4 == 0 ) { // HIGH_YEAR February

            if ( UNIX > _29 ) {
                UNIX -= _29; s_m++;
            }

        } else if ( UNIX > _28 ) { // LOW_YEAR February
            UNIX -= _28; s_m++;
        }


        if ( UNIX > _31 ) { // UNIX -= _31; // March
            UNIX -= _31; s_m++;

            if ( UNIX > _30 ) { // UNIX -= _30; // April
                UNIX -= _30; s_m++;

                if ( UNIX > _31 ) { // UNIX -= _31; // May
                    UNIX -= _31; s_m++;

                    if ( UNIX > _30 ) { // UNIX -= _30; // June
                        UNIX -= _30; s_m++;

                        if ( UNIX > _31 ) { // UNIX -= _31; // July
                            UNIX -= _31; s_m++;

                            if ( UNIX > _31 ) { // UNIX -= _31; // August
                                UNIX -= _31; s_m++;

                                if ( UNIX > _30 ) { // UNIX -= _30; // September
                                    UNIX -= _30; s_m++;

                                    if ( UNIX > _31 ) { // UNIX -= _31; // October
                                        UNIX -= _31; s_m++;

                                        if ( UNIX > _30 ) { // UNIX -= _30; // November
                                            UNIX -= _30; s_m++;

                                            /* if ( UNIX > _31 ) { // UNIX -= _31; // December
                                                UNIX -= _31; s_m++;

                                                *** THIS CAN'T HAPPEND ***

                                            } */
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // ---------------------------------------------------------------
    // NOTE local time diff
    uint32_t LOCAL_TIME_DIFF = 7200;

    UNIX += ( LOCAL_TIME_DIFF );

    while ( C < UNIX ) {
        //  J   F   M   A   M   J   J   A   S   O   N   D
        //  1   2   3   4   5   6   7   8   9   10  11  12
        //  31 [2x] 31  30  31  30  31  31  30  31  30  31

        if ( s_s > 59 ) { // SEC
            s_s = 0;  s_i++;

            if ( s_i > 59 ) { // MIN
                s_i = 0;  s_h++;

                if ( s_h > 23 ) { // HOUR

                    // ----------------------------------------------------------------
                    s_h = 0;  s_d++;

                    // week day index switch
                    s_wd++; if ( s_wd > 6 ) s_wd = 0;

                    // ----------------------------------------------------------------
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

                    // ----------------------------------------------------------------
                    // MONTH switch
                    if ( s_m > 12 ) { s_m = 1; s_y++; }

                    // ----------------------------------------------------------------
                } // HOUR
            } // MIN
        } // SEC

        s_s++;
        C++;

    }

    // BUG s_s--;
    s_s--;


    // ISO 8601 [date -Is]
    // 2013-10-08T10:48:03+0300

    LOCAL_TIME_DIFF /= 3600;


    printf("%d-%d-%dT%d:%d:%d+%d \n", s_y, s_m, s_d, /*T*/ s_h, s_i, s_s, LOCAL_TIME_DIFF  );



    //printf("%s %d-%d-%d %s %d:%d:%d \n", W_D[ s_wd ], s_y, s_m, s_d, MM_S[ s_m-1 ], s_h, s_i, s_s  );


}

// ====================================================================



