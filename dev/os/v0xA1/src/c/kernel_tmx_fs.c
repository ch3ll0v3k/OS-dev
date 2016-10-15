#include "h/kernel.h"

// ====================================================================
uint32_t ONE_LBA            = 512;  // byte
uint16_t MAIN_BASE          = 2;    // offset in LBAs
uint32_t BIT_MAP_LBAs       = 2;    // size in LBAs
uint32_t BIT_MAP_BASE       = 1024; // First bit-map entry at LBAs
uint32_t LBAs_IN_DATA_BLOCK = 8192; // 64 * 128 == 8192

uint16_t ZERO[256];

uint8_t DEBUG_LBA_WRITE     = 0;

// ====================================================================
uint8_t DRIVE_WAIT( uint32_t drive ) {

    // ------------------------------------------------------------
    uint32_t CC = 0, C_B = 0;
    while ( (inb( drive + 7) & 0x80) ) {

        if ( (CC++) == 1000 ) {
            if ( DEBUG_LBA_WRITE ) {
                k_term_hist_off(); k_term_color = K_TERM_L_RED;
                printf(" [DEBUG] [ATA] [ATA_WAIT_WRITE] ['kernel_ext2.c'] \n");
                k_term_hist_on(); k_term_color = K_TERM_WHITE;

            }

            CC = 0;
            C_B++;
        } else if ( C_B >= 100 ) {
            return 0; // 100.000 time without success. return 0;
        }

        __asm__ __volatile__("nop");
    }

    return 1;
    // ------------------------------------------------------------

}


// ====================================================================
uint8_t SET_LBA_WRITE( uint32_t drive, uint32_t ata_lba_mode, uint32_t lba, uint32_t ata_sec_cnt) {

    // ------------------------------------------------------------
    outb( drive + 6, ata_lba_mode | (ata_lba_mode << 4) | ((lba >> 24) & 0x0F));
    outb( drive + 2, (uint8_t)(ata_sec_cnt) );
    outb( drive + 3, (uint8_t)((lba) & 0xFF) );         // lba low byte
    outb( drive + 4, (uint8_t)((lba >> 8) & 0xFF) );    // lba mid byte
    outb( drive + 5, (uint8_t)((lba >> 16) & 0xFF) );   // lba high byte
    outb( drive + 7, ATA_CMD_WRITE ); // 0x30 == Write Sectores (with retries)

    if ( DRIVE_WAIT( drive )) return 1;
    return 0;
    // ------------------------------------------------------------

}

// ====================================================================
uint8_t LBA_WRITE( uint32_t drive, void *data ) {

    // ------------------------------------------------------------
    uint32_t cnt = 256;
    uint16_t *_data = (uint16_t *) data;

    for ( uint32_t i = 0; i < cnt; i++) outw( drive, _data[i] );

    if ( DRIVE_WAIT( drive )) return 1;
    return 0;
    // ------------------------------------------------------------

}

// ====================================================================
uint8_t SET_LBA_READ( uint32_t drive, uint32_t ata_lba_mode, uint32_t lba, uint32_t ata_sec_cnt) {

    // ------------------------------------------------------------
    outb( drive + 6, ata_lba_mode | (ata_lba_mode << 4) | ((lba >> 24) & 0x0F));
    outb( drive + 2, (uint8_t)(ata_sec_cnt) );
    outb( drive + 3, (uint8_t)((lba) & 0xFF) );         // lba low byte
    outb( drive + 4, (uint8_t)((lba >> 8) & 0xFF) );    // lba mid byte
    outb( drive + 5, (uint8_t)((lba >> 16) & 0xFF) );   // lba high byte
    outb( drive + 7, ATA_CMD_READ );   //outb( drive + 7, 0xC4); // 0x29: READ MULTIPLE EXT // 0xC4: READ MULTIPLE SECTORS

    if ( DRIVE_WAIT( drive )) return 1;
    return 0;
    // -----------------------------------------------------------

}

// ====================================================================
uint8_t LBA_READ( uint32_t drive, void *data ) {

    // ------------------------------------------------------------
    uint16_t cnt = 256;
    uint16_t *_data = (uint16_t *) data;

    for ( uint32_t i = 0; i < cnt; i++) _data[i] = inw( drive );

    if ( DRIVE_WAIT( drive )) return 1;

    return 0;
    // ------------------------------------------------------------

}

// ====================================================================
void call_TMX_FS_FORMAT() {
    TMX_FS_FORMAT( 0 );
}

uint8_t TMX_FS_FORMAT( uint16_t hd_num ) {

    // -----------------------------------------------------------
    if ( hd_num >= HDDS_TTL_CNT || HD_INFO[ hd_num ].available == 0 ) {

        printf(" Can't format drive: [0x%x] [%d]\n", HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].available );
        return 0;

    } else {
        printf(" drive: [0x%x] ready ..\n", HD_INFO[ hd_num ].i_drive );
        usleep(250);
    }

    // -----------------------------------------------------------
    for (int i = 0; i < 256; i++) {
        /*
        if ( i % 64 == 0 )
            ZERO[ i ] = (uint16_t) 0xABCD;
        else
            ZERO[ i ] = (uint16_t) 0x00;
        */

        ZERO[ i ] = (uint16_t) 0;
    }

    // -----------------------------------------------------------
    // 1022 * 512 = 523264 byte | from LBA-2 -> LBA-1024 == 1022-LBAs
    // 1022 * 512 / 8 = 65408 64-bit addrs
    // 1022 * 512 / 4 = 130816 32-bit addrs

    printf(" FORMATING: [INFO-BLOCKS] ...\n");

    uint32_t CURR_LBA = 0, SEC_CNT = 0;
    CURR_LBA = MAIN_BASE;


    while ( CURR_LBA < MAIN_BASE+1022 ) { // 1022 LBA

        SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );

        LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );
        CURR_LBA++;

    }

    // -----------------------------------------------------------
    /*
    uint32_t _32_B_H_;
    // _32_B_H_ = 0b11111111111111110000000000000000; // OK
    // _32_B_H_ = 0b00000000000000001111111111111111; // OK
    // _32_B_H_ = 0b00000000111111110000000011111111; // OK
    // _32_B_H_ = 0b11111111000000001111111100000000; // OK
    // _32_B_H_ = 0x12345678; // OK

    uint32_t ZERO_32_BIT[ 128 ];

    for (int i = 0; i < 128; i++) {
        //if ( i % 2 == 0 )
        ZERO_32_BIT[ i ] = _32_B_H_ + (i*2)-1;

    }
    */
    // -----------------------------------------------------------
    uint32_t BIT_MAP_CURR_LBA = 0;

    CURR_LBA = 0;

    printf(" FORMATING: [BIT-BLOCKS] ...\n");
    while ( HD_INFO[ hd_num ].P_lba_capacity > CURR_LBA ) {


        // -----------------------------
        CURR_LBA = BIT_MAP_BASE + ( BIT_MAP_CURR_LBA * LBAs_IN_DATA_BLOCK ); // 1024 + ( [*] * 8192 )
        SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA++, 1 );

        LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );
        // *^* LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO_32_BIT );

        // -----------------------------
        SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 ); // BIT_MAP_LBAs

        LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );
        // *^* LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO_32_BIT );


        BIT_MAP_CURR_LBA++;


    }

    printf("TMX_FS_FORMAT: DRIVE: [0x%x] is formated.\n", HD_INFO[ hd_num ].i_drive );

    // -----------------------------------------------------------
    return 1;
    // -----------------------------------------------------------

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t TMX_FS_MK_DIR( char path[], char name[] ) {

    // ------------------------------------------------------------
    k_term_hist_off();
    // ------------------------------------------------------------




    // ------------------------------------------------------------
    k_term_hist_on();
    return 1;
    // ------------------------------------------------------------
}

// ====================================================================
uint8_t TMX_FS_GET_PWD( char path[], char name[] ) {
    // ------------------------------------------------------------
    return 1;
    // ------------------------------------------------------------
}

// ====================================================================
uint8_t TMX_FS_RM( char path[], char name[] ) {
    // ------------------------------------------------------------
    return 1;
    // ------------------------------------------------------------
}

// ====================================================================
uint8_t TMX_FS_MK_FILE( char path[], char name[] ) {
    // ------------------------------------------------------------
    return 1;
    // ------------------------------------------------------------
}


// ====================================================================
void call_TMX_FS_LIST_DIR() {
    TMX_FS_LIST_DIR( "---",  "---" );

}

uint8_t TMX_FS_LIST_DIR( char path[], char name[] ) {

    // ------------------------------------------------------------
    /* 32-bit Structure */

    uint8_t hd_num = 0;
    printf("-------------------------------------------------\n" );

    uint16_t MAIN_BASE_DATA_0[ 256 ] = { 0 };

    uint32_t ii = 1020, _ADDR_ = 0;

    uint16_t _16bit_H_ = 0, _16bit_L_ = 0;


    while ( MAIN_BASE + ii < 1028 ) {

        SET_LBA_READ( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, MAIN_BASE + ii, 1 );
        LBA_READ( HD_INFO[ hd_num ].i_drive, (void *)MAIN_BASE_DATA_0 );

        uint16_t i = 0;
        while ( i < 256 ) {

            _16bit_H_ = MAIN_BASE_DATA_0[ i+1 ]; // BIT-REVERCED
            _16bit_L_ = MAIN_BASE_DATA_0[ i ];   // BIT-REVERCED

            _ADDR_ = ( MAIN_BASE_DATA_0[ i+1 ] << 16 ) | ( MAIN_BASE_DATA_0[ i ] );

            if ( _ADDR_ != 0 )
                printf("[0x%x -> 0x%x] >> [0x%x] \n", _16bit_H_, _16bit_L_, _ADDR_ );

            i += 2;

        }

        usleep(10);

        ii++;

    }

    return 1;

    // ------------------------------------------------------------
    /* 16-bit Structure

    uint8_t hd_num = 0;
    printf("-------------------------------------------------\n" );

    uint16_t MAIN_BASE_DATA_0[ 256 ] = { 0 };

    uint32_t ii = 1020;

    while ( MAIN_BASE + ii < 1028 ) {

        SET_LBA_READ( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, MAIN_BASE + ii, 1 );
        LBA_READ( HD_INFO[ hd_num ].i_drive, (void *)MAIN_BASE_DATA_0 );

        printf("------------------------\nLBA-[%d]\n", MAIN_BASE + ii );
        for (int i = 0; i < 256; i++)
            if ( MAIN_BASE_DATA_0[i] != 0 ) printf("[%d] -> [0x%x]\n", ii, MAIN_BASE_DATA_0[i] );

        usleep(10);


        for ( uint16_t ix=0; ix<256; ix++) {
            MAIN_BASE_DATA_0[ ix ] = 0;
        }


        ii++;

    }

    return 1;
    */
    // ------------------------------------------------------------
}

// ====================================================================
uint8_t TMX_FS_CAT( char path[], char name[] ) {
    // ------------------------------------------------------------
    return 1;
    // ------------------------------------------------------------
}

// ====================================================================
