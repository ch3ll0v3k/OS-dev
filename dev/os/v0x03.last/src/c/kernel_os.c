#include "h/kernel.h"

// ====================================================================

void __OS__() {
    char OS_N[] = "TMX-OS KERNEL v 0.0.1";

    uint16_t len = strlen( OS_N );

    if ( len % 2 != 0 ) len++;

    char *BUFF = (char *)0xb8000 + ((TERM_WD * TERM_H/2) - (len)  );

    uint16_t i=0;
    uint16_t i_2=0;

    while ( i< len ) {

        BUFF[i_2++] = OS_N[i++];
        BUFF[i_2++] = 0x7;
        usleep(100);

    }

    BUFF[i_2++] = ' ';
    BUFF[i_2++] = 0x7;

    i=0;
    while ( i < 10 ) {

        if ( i%2 == 0 ) {
            BUFF[i_2] = ' ';

        } else {
            BUFF[i_2] = '#';
        }

        BUFF[i_2+1] = 0x7;
        usleep(150);
        i++;

    }

    sleep(1);
    k_term_clear();

}

// ====================================================================


