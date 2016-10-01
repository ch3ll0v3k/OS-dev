#include "h/kernel.h"

// ====================================================================
void ATA_WAIT_400ns( uint16_t ATA_CTRL ) {

    // inb( 0x3F6 ); // wait 100 Nano-Seconds
    inb( ATA_CTRL ); inb( ATA_CTRL ); inb( ATA_CTRL ); inb( ATA_CTRL );

}

void ATA_GET_READY( uint16_t ATA, uint16_t ATA_CTRL ) {

    ATA_0_STATUS = inb( ATA + 7);

    while (ATA_0_STATUS & 0x80) {
        printf(" [ATA] [BSY] WAIT(400 ns)\n" );
        inb( ATA_CTRL );
        inb( ATA_CTRL );
        inb( ATA_CTRL );
        inb( ATA_CTRL );

        ATA_0_STATUS = inb( ATA + 7);
    }

}

void ATA_0_HANDLER( REG *reg ) {

    // int16_t clr = k_term_color;
    // k_term_color = 0xF0;

    // k_term_printnl(" [ATA][0] HANDLER: [CALL]");
    // if ( reg->int_no > 10000 && reg->err_code > 10000 ) return;

    // k_term_color = clr;

}

void ATA_1_HANDLER( REG *reg ) {

    int16_t clr = k_term_color;
    k_term_color = 0xF0;

    k_term_printnl(" [ATA][1] HANDLER: [CALL]");
    if ( reg->int_no > 10000 && reg->err_code > 10000 ) return;

    k_term_color = clr;
}


void INSTALL_ATA_HANDLERS() {

    ATA_LBA = 0;
    ATA_SEC_CNT = 0;


    int16_t clr = k_term_color;
    k_term_color = K_TERM_L_GREEN;


    printf(" [ATA][0] HANDLER: INSTALL\n");
    irq_install_handler(14, ATA_0_HANDLER );

    printf(" [ATA][1] HANDLER: INSTALL\n");
    irq_install_handler(15, ATA_1_HANDLER );
    //sleep(3);

    k_term_color = clr;
}


// ====================================================================
