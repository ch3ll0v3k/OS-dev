#include "h/kernel.h"

// ====================================================================
uint64_t ata_0_ttl_b;
uint64_t ata_0_ttl_kb;
uint32_t ata_0_ttl_mb;
uint32_t ata_0_ttl_gb;

uint8_t DEBUG_LBA_SEC = 1;
// ====================================================================
void set_LBA_SEC_CNT( uint32_t _ATA_LBA, uint32_t _ATA_SEC_CNT );

// ====================================================================
uint8_t init_ata_utils() {

    // ----------------------------------------------------------------
    ATA_UTILS_INITED = 0;
    HDDS_TTL_CNT = 0;

    // ATA MASTER/SLAVE = 0xE0 / 0xF0
    // ATA 0/1 = 0x1F0 / 0x170

    hdd_info( ATA_0, ATA_MASTER ); HDDS_TTL_CNT++;
    hdd_info( ATA_0, ATA_SLAVE  ); HDDS_TTL_CNT++;
    hdd_info( ATA_1, ATA_MASTER ); HDDS_TTL_CNT++;
    hdd_info( ATA_1, ATA_SLAVE  ); HDDS_TTL_CNT++;

    // ----------------------------------------------------------------

    return ( ++ATA_UTILS_INITED );
    // ----------------------------------------------------------------

}


// ====================================================================

// Cache Flush

// On some drives it is necessary to "manually" flush the hardware write cache after
// every write command. This is done by sending the 0xE7 command to the Command Register
// (then waiting for BSY to clear).

// If a driver does not do this, then subsequent write commands can fail invisibly,
// or "temporary bad sectors" can be created on your disk.

// [512 byte]
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do.
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void set_LBA_SEC_CNT_WRITE( uint32_t _ATA_LBA ) {

    // ----------------------------------------------------------------
    uint8_t _ATA_SEC_CNT = 1;
    // uint32_t _ATA_LBA = 0;

    // ------------------------------------------------------------
    outb( ATA_0 + 6, ATA_LBA_MODE | (ATA_MASTER << 4) | ((_ATA_LBA >> 24) & 0x0F));

    outb( ATA_0 + 2, (uint8_t)(_ATA_SEC_CNT) );
    //outb( ATA_0 + 1, 0x00 );
    //outb( ATA_0 + 2, 0x01 );

    outb( ATA_0 + 3, (uint8_t)((_ATA_LBA) & 0xFF) );         // _ATA_LBA low byte
    outb( ATA_0 + 4, (uint8_t)((_ATA_LBA >> 8) & 0xFF) );    // _ATA_LBA mid byte
    outb( ATA_0 + 5, (uint8_t)((_ATA_LBA >> 16) & 0xFF) );   // _ATA_LBA high byte

    outb( ATA_0 + 7, ATA_CMD_WRITE ); // 0x30 == Write Sectores (with retries)

    // ------------------------------------------------------------
    uint32_t CC = 0;
    while ( (inb( ATA_0 + 7) & 0x80) ) {

        if ( (CC++) == 1000 ) {

            if ( DEBUG_LBA_SEC ) {

                k_term_hist_off();
                k_term_color = K_TERM_L_RED; // K_TERM_L_RED == 0xc
                printf(" [DEBUG] [ATA] [ATA_WAIT_WRITE] ['kernel_ext2.c'] \n");
                k_term_color = K_TERM_WHITE;
                k_term_hist_on();

            }

            CC = 0;
        }

        __asm__ __volatile__("nop");
    }

    // ------------------------------------------------------------
    printf(" [DEBUG] [ATA] [ATA_WRITE] \n");


    uint8_t DATA[512] = { "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do." };

    uint32_t cnt_read = 256;

    uint16_t *ptr = (uint16_t *) DATA;



    for ( uint32_t i = 0; i < cnt_read; i++) {

        k_term_hist_off();
        k_term_hist_on();

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );

        outw( ATA_0, *ptr++ );

    }

    printf(" [DEBUG] [ATA] [ATA_WRITE] done \n");

    // ------------------------------------------------------------


}

// ====================================================================
void READ_CHAR_T_512b() {


    set_LBA_SEC_CNT( 0, 1 );

    // -----------------------------------------------------------
    uint32_t cnt_read = 256;
    uint16_t DATA[ cnt_read ];

    for ( uint32_t i = 0; i < cnt_read; i++) {

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );

        DATA[i] = inw( ATA_0 );

    }

    char *ptr = (char *) DATA;
    ptr[ strlen(ptr) ] = 0;


    printf("strlen( ptr / DATA ): [%d] [%d]\n", strlen(ptr), strlen((char *)DATA) );

    uint32_t _i = 0;


    while ( _i < 512 ) {

        printf("%c", ptr[_i++] );

    }

    printf(" \n " );
    //printf("[%s]\n", ptr );



}

// ====================================================================
void set_LBA_SEC_CNT( uint32_t _ATA_LBA, uint32_t _ATA_SEC_CNT ) {

    // ------------------------------------------------------------
    // Suppose we have a hard drive with 512 bytes per sector (or bytes per block).
    // LBA of 50 would specify the sector containing bytes (50 * 512) to (51 * 512 - 1)

    //_ATA_SEC_CNT = 1;
    // ------------------------------------------------------------
    outb( ATA_0 + 2, (uint8_t)(_ATA_SEC_CNT) );
    outb( ATA_0 + 3, (uint8_t)((_ATA_LBA) & 0xFF) );         // _ATA_LBA low byte
    outb( ATA_0 + 4, (uint8_t)((_ATA_LBA >> 8) & 0xFF) );    // _ATA_LBA mid byte
    outb( ATA_0 + 5, (uint8_t)((_ATA_LBA >> 16) & 0xFF) );   // _ATA_LBA high byte
    //outb( ATA_0 + 6, (_ATA_LBA >> 24) & 0x0F); //bits 24-28 of LBA
    outb( ATA_0 + 6, ATA_LBA_MODE | (ATA_MASTER << 4) | ((_ATA_LBA >> 24) & 0x0F));
    outb( ATA_0 + 7, 0x20);   //outb( ATA_0 + 7, 0xC4); // 0x29: READ MULTIPLE EXT // 0xC4: READ MULTIPLE SECTORS

    // -----------------------------------------------------------
    //ATA_WAIT_400ns( ATA_0_CTRL ); // ATA_GET_READY( ATA_0, ATA_0_CTRL );

    uint32_t CC = 0;
    while ( (inb( ATA_0 + 7) & 0x80) ) {

        if ( (CC++) == 1000 ) {

            if ( DEBUG_LBA_SEC ) {

                k_term_hist_off();
                k_term_color = K_TERM_L_RED; // K_TERM_L_RED == 0xc
                printf(" [DEBUG] [ATA] [ATA_WAIT_READ] ['kernel_ext2.c'] \n");
                k_term_color = K_TERM_WHITE;
                k_term_hist_on();

            }

            CC = 0;
        }

        __asm__ __volatile__("nop");
    }

    //ide_poll();

    //ATA_0_STATUS = inb( ATA_0 + 7);
    // if (ATA_0_STATUS & 0x80)  { printf(" [EXT2] [BSY]\n" ); return; }
    // if (ATA_0_STATUS & 0x1)  { printf(" [EXT2] [ERR]\n" ); return; }
    // if (!(ATA_0_STATUS & 0x8))  { printf(" [EXT2] [DRQ]\n" ); return; }
    // if (ATA_0_STATUS & 0x20) { printf(" [EXT2] [DF]\n" ); return; }
    // if (ATA_0_STATUS & 0x88) { printf(" [EXT2] [BSY && DRQ]\n" ); return; }

    // -----------------------------------------------------------

}

// ====================================================================
/*
uint8_t read() {

    // -----------------------------------------------------------
    uint8_t DEBUG = 0;

    //set_LBA_SEC_CNT( ATA_LBA, ATA_SEC_CNT );

    // -----------------------------------------------------------
    uint32_t cnt_read = 256;
    uint16_t DATA[ cnt_read ]; // ext2_superblock_t

    for ( uint32_t i = 0; i < cnt_read; i++) {

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );

        DATA[i] = inw( ATA_0 );

    }

    return 1;

    // -----------------------------------------------------------

}
*/

// ====================================================================
uint8_t get_hd_info( uint8_t i) {

    // -----------------------------------------------------------
    if ( i >= HDDS_TTL_CNT || HD_INFO[i].available == 0 ) {

        printf(" ----------------------------------------------------- \n" );
        printf(" DRIVE:[%d] NOT-AVAILABLE\n", i );

        return 0;
    }

    // -----------------------------------------------------------
    printf(" ----------------------------------------------------- \n" );
    printf(" DRIVE:[%d] mode:[0x%x]\n", i, HD_INFO[i].i_mode );

    printf( "['%d'].i_drive:            [%d] [0x%x]\n", i, HD_INFO[i].i_drive, HD_INFO[i].i_drive );
    printf( "['%d'].i_mode:             [%d] [0x%x]\n", i, HD_INFO[i].i_mode, HD_INFO[i].i_mode );
    printf( "['%d'].s_drive:            [%s]\n", i, HD_INFO[i].s_drive );
    printf( "['%d'].s_mode:             [%s]\n", i, HD_INFO[i].s_mode );

    printf( "['%d'].L_cyls:             [%d] [0x%x]\n", i, HD_INFO[i].L_cyls, HD_INFO[i].L_cyls );
    printf( "['%d'].L_heads:            [%d] [0x%x]\n", i, HD_INFO[i].L_heads, HD_INFO[i].L_heads );
    printf( "['%d'].L_sectors:          [%d] [0x%x]\n", i, HD_INFO[i].L_sectors, HD_INFO[i].L_sectors );
    printf( "['%d'].L_capacity0:        [%d] [0x%x]\n", i, HD_INFO[i].L_capacity0, HD_INFO[i].L_capacity0 );

    printf( "['%d'].P_cyls:             [%d] [0x%x]\n", i, HD_INFO[i].P_cyls, HD_INFO[i].P_cyls );
    printf( "['%d'].P_heads:            [%d] [0x%x]\n", i, HD_INFO[i].P_heads, HD_INFO[i].P_heads );
    printf( "['%d'].P_sectors:          [%d] [0x%x]\n", i, HD_INFO[i].P_sectors, HD_INFO[i].P_sectors );
    printf( "['%d'].P_lba_capacity:     [%d] [0x%x]\n", i, HD_INFO[i].P_lba_capacity, HD_INFO[i].P_lba_capacity );

    printf( "['%d'].size_MiB:           [%d] [0x%x]\n", i, HD_INFO[i].size_MiB, HD_INFO[i].size_MiB );
    printf( "['%d'].size_GiB:           [%d] [0x%x]\n", i, HD_INFO[i].size_GiB, HD_INFO[i].size_GiB );
    //HD_INFO[0].model          = HD.model;
    //HD_INFO[0].serial_no      = HD.serial_no;

    return 1;

    // ------------------------------------------------------------
}

// ====================================================================
uint8_t hdd_info( int32_t drive, int32_t mode) {

    // ------------------------------------------------------------
    uint8_t DEBUG_HDD_TEST = 0;

    if ( DEBUG_HDD_TEST )
        printf("CONTROLLER: [0x%x] READY: [0x%x] \n", drive + 7, inb(drive + 7) );

    outb(drive + 6, mode); // master / slave select
    outb(drive + 7, ATA_CMD_IDENT); // [0xEC] Identify Device

    // ---------------------------------------------

    uint32_t CNT = 0;
    uint32_t CNT_MAX = 4096;

    while ( inb(drive + 7) != ATAP_READY_SPP ) { // 0x58 01011000b which means the busy bit is clear, the RDY bit is set

        if ( DEBUG_HDD_TEST )
            printf("CONTROLLER: [0x%x] READY: [0x%x] \n", drive + 7, inb(drive + 7) );

        __asm__ __volatile__ ("nop");

        CNT++;
        if ( CNT >= CNT_MAX ) {

            HD_INFO[ HDDS_TTL_CNT ].available = 0;

            // printf(" ----------------------------------------------------- \n " );
            // printf(" [DATA - drive:[0x%d] mode:[0x%x]\n", drive, mode );
            // printf(" drive not available !" );
            return 0;

        }

    }

    uint16_t data[256];

    for ( uint16_t i = 0; i<256; i++) { // Read the data
        data[i] = inw(drive);
    }


    hd_drive_id_t HD = *((hd_drive_id_t*) data);
    uint8_t cp = HD.capability;

    if ( DEBUG_HDD_TEST ) {

        printf(" ----------------------------------------------------- \n");
        printf("vendor[0-6]:        [%d %d %d %d %d %d] \n", HD.vendor0, HD.vendor1, HD.vendor2, HD.vendor3, HD.vendor4, HD.vendor5, HD.vendor6 ); // [ATA_IDENT] bits 0:DMA 1:LBA 2:IORDYsw 3:IORDYsu

    }

    // ----------------------------------------------------
    HD_INFO[ HDDS_TTL_CNT ].available        = 1;

    HD_INFO[ HDDS_TTL_CNT ].i_drive          = drive;
    HD_INFO[ HDDS_TTL_CNT ].i_mode           = mode;

    sprintf( HD_INFO[ HDDS_TTL_CNT ].s_drive, "%s", drive == ATA_0 ? "ATA_0" : drive == ATA_1 ? "ATA_1" : "ATA_x" );
    sprintf( HD_INFO[ HDDS_TTL_CNT ].s_mode,  "%s", mode == ATA_MASTER ? "MASTER" : "SLAVE" );

    HD_INFO[ HDDS_TTL_CNT ].L_cyls           = HD.cur_cyls; //  logical cylinders
    HD_INFO[ HDDS_TTL_CNT ].L_heads          = HD.cur_heads; //  logical heads
    HD_INFO[ HDDS_TTL_CNT ].L_sectors        = HD.cur_sectors; //  logical sectors per track
    HD_INFO[ HDDS_TTL_CNT ].L_capacity0      = HD.cur_capacity0; //  logical total sectors on drive

    HD_INFO[ HDDS_TTL_CNT ].P_cyls           = HD.cyls; //  physical cylinders
    HD_INFO[ HDDS_TTL_CNT ].P_heads          = HD.heads; //  physical heads
    HD_INFO[ HDDS_TTL_CNT ].P_sectors        = HD.sectors; //  physical sectors per track
    HD_INFO[ HDDS_TTL_CNT ].P_lba_capacity   = HD.lba_capacity; // total number of sectors
    HD_INFO[ HDDS_TTL_CNT ].size_MiB         = HD.lba_capacity * 512 / 1024 / 1024;
    HD_INFO[ HDDS_TTL_CNT ].size_GiB         = HD.lba_capacity * 512 / 1024 / 1024 / 1024;

    sprintf( HD_INFO[ HDDS_TTL_CNT ].capability, "%s", (cp == 3) ? "IORDYsu" : (cp == 2) ? "IORDYsw" : (cp == 1) ? "LBA" : "DMA" );
    sprintf( HD_INFO[ HDDS_TTL_CNT ].tDMA, "%s", (HD.tDMA == 2) ? "FAST" : (HD.tDMA == 1) ? "MEDD" : "SLOW" );

    //HD_INFO[ HDDS_TTL_CNT ].model          = HD.model;
    //HD_INFO[ HDDS_TTL_CNT ].serial_no      = HD.serial_no;
    // ----------------------------------------------------

    /*
    uint16_t    config;                     // [ATA_IDENT] lots of obsolete bit flags
    uint16_t    cyls;                       // [ATA_IDENT] "physical" cyls
    uint16_t    reserved2;                  // [ATA_IDENT] reserved (word 2)
    uint16_t    heads;                      // [ATA_IDENT] "physical" heads
    uint16_t    track_bytes;                // [ATA_IDENT] unformatted bytes per track
    uint16_t    sector_bytes;               // [ATA_IDENT] unformatted bytes per sector
    uint16_t    sectors;                    // [ATA_IDENT] "physical" sectors per track
    uint16_t    vendor0;                    // [ATA_IDENT] vendor unique
    uint16_t    vendor1;                    // [ATA_IDENT] vendor unique
    uint16_t    vendor2;                    // [ATA_IDENT] vendor unique
    uint8_t     serial_no[20];              // [ATA_IDENT] 0 = not_specified
    uint16_t    buf_type;                   // [ATA_IDENT]  +2
    uint16_t    buf_size;                   // [ATA_IDENT] 512 byte increments; 0 = not_specified
    uint16_t    ecc_bytes;                  // [ATA_IDENT] for r/w long cmds; 0 = not_specified
    uint8_t     fw_rev[8];                  // [ATA_IDENT] 0 = not_specified
    uint8_t     model[40];                  // [ATA_IDENT] 0 = not_specified
    uint8_t     max_multsect;               // [ATA_IDENT] 0=not_implemented
    uint8_t     vendor3;                    // [ATA_IDENT] vendor unique
    uint16_t    dword_io;                   // [ATA_IDENT] 0=not_implemented; 1=implemented
    uint8_t     vendor4;                    // [ATA_IDENT] vendor unique
    uint8_t     capability;                 // [ATA_IDENT] bits 0:DMA 1:LBA 2:IORDYsw 3:IORDYsu
    uint16_t    reserved50;                 // [ATA_IDENT] reserved (word 50)
    uint8_t     vendor5;                    // [ATA_IDENT] vendor unique
    uint8_t     tPIO;                       // [ATA_IDENT] 0=slow, 1=medium, 2=fast
    uint8_t     vendor6;                    // [ATA_IDENT] vendor unique
    uint8_t     tDMA;                       // [ATA_IDENT] 0=slow, 1=medium, 2=fast
    uint16_t    field_valid;                // [ATA_IDENT] bits 0:cur_ok 1:eide_ok
    uint16_t    cur_cyls;                   // [ATA_IDENT] logical cylinders
    uint16_t    cur_heads;                  // [ATA_IDENT] logical heads
    uint16_t    cur_sectors;                // [ATA_IDENT] logical sectors per track
    uint16_t    cur_capacity0;              // [ATA_IDENT] logical total sectors on drive
    uint16_t    cur_capacity1;              // [ATA_IDENT] (2 words, misaligned int)
    uint8_t     multsect;                   // [ATA_IDENT] current multiple sector count
    uint8_t     multsect_valid;             // [ATA_IDENT] when (bit0==1) multsect is ok
    uint32_t    lba_capacity;               // [ATA_IDENT] total number of sectors
    uint16_t    dma_1word;                  // [ATA_IDENT] single-word dma info
    uint16_t    dma_mword;                  // [ATA_IDENT] multiple-word dma info
    uint16_t    eide_pio_modes;             // [ATA_IDENT] bits 0:mode3 1:mode4
    uint16_t    eide_dma_min;               // [ATA_IDENT] min mword dma cycle time (ns)
    uint16_t    eide_dma_time;              // [ATA_IDENT] recommended mword dma cycle time (ns)
    uint16_t    eide_pio;                   // [ATA_IDENT] min cycle time (ns), no IORDY
    uint16_t    eide_pio_iordy;             // [ATA_IDENT] min cycle time (ns), with IORDY
    uint16_t    words69_70[2];              // [ATA_IDENT] reserved words 69-70
    uint16_t    words71_74[4];              // [ATA_IDENT] reserved words 71-74
    uint16_t    queue_depth;                // [ATA_IDENT] +2
    uint16_t    words76_79[4];              // [ATA_IDENT] reserved words 76-79
    uint16_t    major_rev_num;              // [ATA_IDENT] +2
    uint16_t    minor_rev_num;              // [ATA_IDENT] +2
    uint16_t    command_set_1;              // [ATA_IDENT] bits 0:Smart 1:Security 2:Removable 3:PM
    uint16_t    command_set_2;              // [ATA_IDENT] bits 14:Smart Enabled 13:0 zero 10:lba48 suppor
    uint16_t    cfsse;                      // [ATA_IDENT] command set-feature supported extensions
    uint16_t    cfs_enable_1;               // [ATA_IDENT] command set-feature enabled
    uint16_t    cfs_enable_2;               // [ATA_IDENT] command set-feature enabled
    uint16_t    csf_default;                // [ATA_IDENT] command set-feature default
    uint16_t    dma_ultra;                  // [ATA_IDENT]  +2
    uint16_t    word89;                     // [ATA_IDENT] reserved (word 89)
    uint16_t    word90;                     // [ATA_IDENT] reserved (word 90)
    uint16_t    CurAPMvalues;               // [ATA_IDENT] current APM values
    uint16_t    word92;                     // [ATA_IDENT] reserved (word 92)
    uint16_t    hw_config;                  // [ATA_IDENT] hardware config
    uint16_t    words94_99[6];              // [ATA_IDENT] reserved words 94-99
    uint16_t    lba48_capacity[4];          // [ATA_IDENT] 4 16bit values containing lba 48 total number of sectors
    uint16_t    words104_125[22];           // [ATA_IDENT] reserved words 104-125
    uint16_t    last_lun;                   // [ATA_IDENT] reserved (word 126)
    uint16_t    word127;                    // [ATA_IDENT] reserved (word 127)
    uint16_t    dlf;                        // [ATA_IDENT] device lock function
    uint16_t    csfo;                       // [ATA_IDENT] current set features options
    uint16_t    words130_155[26];           // [ATA_IDENT] reserved vendor words 130-155
    uint16_t    word156;                    // [ATA_IDENT] +2
    uint16_t    words157_159[3];            // [ATA_IDENT] reserved vendor words 157-159
    uint16_t    words160_162[3];            // [ATA_IDENT] reserved words 160-162
    uint16_t    cf_advanced_caps;           // [ATA_IDENT] +2
    uint16_t    words164_255[92];           // [ATA_IDENT] reserved words 164-255
    */

    return 1;
    // ------------------------------------------------------------
}

// ====================================================================
