#include "h/kernel.h"

// ====================================================================
uint32_t ONE_LBA            = 512;  // byte
uint16_t MAIN_BASE          = 2;    // offset in LBAs
uint32_t BIT_MAP_LBAs       = 2;    // size in LBAs
uint32_t BIT_MAP_BASE       = 1024; // First bit-map entry at LBAs
uint32_t LBAs_IN_DATA_BLOCK = 8192; // 64 * 128 == 8192

uint16_t ZERO[256]          = { 0 };

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

    // ------------------------------------------------------------
    // for (int i = 0; i < 256; i++) ZERO[ i ] = (uint16_t) 0;

    if ( hd_num >= HDDS_TTL_CNT || HD_INFO[ hd_num ].available == 0 ) {
        printf(" Can't format drive: [0x%x] [%d]\n", HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].available );
        return 0;

    } else {
        printf(" drive: [0x%x] ready ..\n", HD_INFO[ hd_num ].i_drive );
        usleep(250);
    }

    printf(
        "TMX_FS_FORMAT: DRIVE: [0x%x] -> MODE: [0x%x]\n\tS_D: [%s]\n\tS_M: [%s]\n\tDMA: [%s]\n\tCAP: [%s]\n\tLBA: [%d]\n\tMiB: [%d]\n\tGiB: [%d]\n\n",
        HD_INFO[ hd_num ].i_drive,
        HD_INFO[ hd_num ].i_mode,

        HD_INFO[ hd_num ].s_drive,
        HD_INFO[ hd_num ].s_mode,
        HD_INFO[ hd_num ].tDMA,
        HD_INFO[ hd_num ].capability,

        HD_INFO[ hd_num ].P_lba_capacity,
        HD_INFO[ hd_num ].size_MiB,
        HD_INFO[ hd_num ].size_GiB

    );

    usleep( 250 );
    /*  -----------------------------------------------------------
    (1024 * 8) * 64            == 524288 LBAs
    (1024 * 8) * 64 / 2        == 262144 KiB
    (1024 * 8) * 64 / 2 / 1024 == 256    MiB


    HD -> LBAs
    ----------

        BASE-OFFESET  [+4]
        ------------
            0    -> +2      RESERVED-SYS
            2    -> +2      RESERVED-FS

        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        BASE-OFFESET ++ GROUP-BLOCK [0] == 64 MiB: [+ 1024 + (8192 * 64) ] == 66560
        ------------

        GROUP-BLOCK [0]: [+1024] [+65536] [=66560]

            ADDR-BLOCK [0]: [+1024]
                4    -> +1024

            ----------------------------------------------------------------------
            GROUP [0]: [+8192]
                1028 -> +2      BIT-BLOCK
                1030 -> +8190   DATA-BLOCK

            GROUP [1]: [+8192]
                9220 -> +2      BIT-BLOCK
                9222 -> +8190   DATA-BLOCK

            - - -                              TTL: 64 GROUPs x 4 MiB == 256 MiB

            GROUP [62]: [+8192]
                508932 -> +2      BIT-BLOCK
                508934 -> +8190   DATA-BLOCK

            GROUP [63]: [+8192]
                517124 -> +2      BIT-BLOCK
                525316 -> +8190   DATA-BLOCK
            ----------------------------------------------------------------------

        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    */
    // -----------------------------------------------------------
    uint32_t CURR_LBA = 0;
    uint32_t LBA_DIST = 0;
    //uint32_t FISRT_GROUP_LBAs = 66564;


    // ****** ****** ****** ****** ****** ****** ******
    uint32_t SIZE_OF_SYS_HEADER = 2;
    LBA_DIST += SIZE_OF_SYS_HEADER;

    printf("\tHDD[0][%d] -> SYS-HEADER\n", CURR_LBA);
    while ( CURR_LBA +1 < LBA_DIST ) {

        //printf(" HDD[0][%d] -> SYS-HEADER\n", CURR_LBA);

        // SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );
        // LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );

        CURR_LBA++;

    }


    // ****** ****** ****** ****** ****** ****** ******
    printf("\tHDD[0][%d] PARTITION-HEADER: FORMATING: LBA: [%d]\n", hd_num, CURR_LBA);

    uint32_t SIZE_OF_PARTITION_HEADER = 2;
    LBA_DIST += SIZE_OF_PARTITION_HEADER;

    while ( CURR_LBA +1 < LBA_DIST ) {
        //printf(" HDD[0][%d] -> PARTITION-HEADER\n", CURR_LBA);

        // SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );
        // LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );

        CURR_LBA++;

    }


    // ****** ****** ****** ****** ****** ****** ******
    printf("\tADDR-SPACE: FORMATING: LBA:[ %d ] ...\n\n", CURR_LBA);

    uint32_t SIZE_OF_ADDR_SPACE = 1024; // current offset 4 LBAs
    LBA_DIST += SIZE_OF_ADDR_SPACE;

    while ( CURR_LBA +1 < LBA_DIST ) {

        if ( CURR_LBA % 256 == 0 )
            printf("\t\t\tADDR-SPACE: LBA: [ %d ] of  [ %d ] \n", CURR_LBA, SIZE_OF_ADDR_SPACE );

        SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );
        LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );

        CURR_LBA++;

    }

    // ****** ****** ****** ****** ****** ****** ******
    // current offset [4 + 1024] LBAs
    printf("\n\tDATA-GROUP: [ 0-63 ] FORMATING: LBA: [%d] ...\n\n", CURR_LBA );

    int grp_cnt = 0;

    for (; grp_cnt < 64; grp_cnt++ ) {


        //if ( grp_cnt % 16 == 0 ) // 1[64], 2[32], 4[16], 8[8], 16[4], 32[2], 64[1-0] -1
        //    printf("\t\tDATA-GROUP: [ %d ] LBA: [%d] MiB: [%d]\n", grp_cnt, CURR_LBA, (CURR_LBA - 1024 - 2 - 2) / 2 / 1024);

        SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA++, 1 );
        LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );

        SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA++, 1 );
        LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );

        CURR_LBA += 8190; // LBA DATA-BLOCK (just skip)

    }

    printf("\n DRIVE: [0x%x] FORMATED\n\t [%d] of [%d]\n\n",
           HD_INFO[ hd_num ].i_drive, CURR_LBA, HD_INFO[ hd_num ].P_lba_capacity );



    return 1;
    // ------------------------------------------------------------
    /*
    SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );

    LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );


    uint32_t BIT_MAP_CURR_LBA = 0;

    while ( HD_INFO[ hd_num ].P_lba_capacity > CURR_LBA ) {


        // -----------------------------
        CURR_LBA = BIT_MAP_BASE + ( BIT_MAP_CURR_LBA * LBAs_IN_DATA_BLOCK ); // 1024 + ( [*] * 8192 )
        SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );
        LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );

        // -----------------------------
        CURR_LBA += 1; // +
        SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 ); // BIT_MAP_LBAs
        LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ZERO );


        BIT_MAP_CURR_LBA++;


    }
    */

    // -----------------------------------------------------------
    printf("TMX_FS_FORMAT: DRIVE: [0x%x] is formated.\n", HD_INFO[ hd_num ].i_drive );

    return 1;
    // -----------------------------------------------------------

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t TMX_FS_GET_PWD( char path[], char name[] ) {
    // ------------------------------------------------------------
    return path[0]+name[0];
    // ------------------------------------------------------------
}

// ====================================================================
uint8_t TMX_FS_RM( char path[], char name[] ) {
    // ------------------------------------------------------------
    return path[0]+name[0];
    // ------------------------------------------------------------
}

// ====================================================================
uint8_t TMX_FS_MK_FILE( char path[], char name[] ) {
    // ------------------------------------------------------------
    return path[0]+name[0];
    // ------------------------------------------------------------
}


// ====================================================================
void call_TMX_FS_LIST_DIR() {
    TMX_FS_LIST_DIR( "---",  "---" );

}

uint8_t TMX_FS_LIST_DIR( char path[], char name[] ) {

    // ------------------------------------------------------------
    uint8_t hd_num = 0;
    uint16_t MAIN_BASE_DATA_0[ 256 ] = { 0 };
    uint32_t _ADDR_ = 0;

    uint16_t _16bit_H_ = 0, _16bit_L_ = 0;

    uint32_t SIZE_OF_ADDR_SPACE = 1024;
    uint32_t CURR_LBA = 0;


    // printf(" skipping header: [4 x LBAs]\n");
    while ( CURR_LBA + 1 < 4 ) { // HEADERS_TTL
        CURR_LBA++;

    }

    //printf(" getting : [4 x LBAs] START LBA: [%d]\n", CURR_LBA );
    while ( CURR_LBA < SIZE_OF_ADDR_SPACE ) { // SIZE_OF_ADDR_SPACE == 1024 LBAs

        SET_LBA_READ( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );
        LBA_READ( HD_INFO[ hd_num ].i_drive, (void *)MAIN_BASE_DATA_0 );


        uint16_t i = 0;

        while ( i < 256 ) {

            _16bit_H_ = MAIN_BASE_DATA_0[ i+1 ]; // BIT-REVERCED
            _16bit_L_ = MAIN_BASE_DATA_0[ i ];   // BIT-REVERCED

            _ADDR_ = ( _16bit_H_ << 16 ) | ( _16bit_L_ );


            if ( _ADDR_ == 0x00 && i == 0x00 ) {
                printf(" ['/'] is empty. \n");
                break;

            } else if ( _ADDR_ != 0x00 ) {

                // printf("\t[0x%x -> 0x%x] >> [0x%x] \n", _16bit_H_, _16bit_L_, _ADDR_ );
                get_file_info( _ADDR_ );


            } else {
                break;

            }

            i += 2;

        }

        break; // just first ADDR-BLOCK: LBA
        usleep(10);

    }

    return path[0]+name[0];

    // ------------------------------------------------------------
}

// ====================================================================
uint8_t get_file_info( uint32_t addr ) {

    // ------------------------------------------------------------
    //printf(" ------------------------------ \n" );
    //printf("get_file_info: [0x%x]\n", addr );

    uint16_t _16_BIT_ARR[ 256 ] = { 0 };
    uint32_t CURR_LBA = addr;

    SET_LBA_READ( HD_INFO[ 0 ].i_drive, HD_INFO[ 0 ].i_mode, CURR_LBA, 1 );
    LBA_READ( HD_INFO[ 0 ].i_drive, (void *)_16_BIT_ARR );

    //printf(" [%s] \n", (char *) _16_BIT_ARR );

    tmx_fs_file_node_t file_node =  *(( tmx_fs_file_node_t *)_16_BIT_ARR);
    file_node.i_name[ file_node.i_name_l ] = 0;

    //printf( "i_name:        [%s]\n",        file_node.i_name );

    // k_term_print( "i_name:        [" );
    // uint8_t _c=0;
    // while ( file_node.i_name[_c] )
    //      k_term_print( file_node.i_name[_c++] );
    // k_term_printnl( "]" );

    // printf( "i_name_l:      [%d]:[0x%x]\n", file_node.i_name_l );
    // printf( "i_parent:      [%d]:[0x%x]\n", file_node.i_parent );
    // printf( "i_type:        [%d]:[0x%x]\n", file_node.i_type );
    // i_type >> 16 == type
    // i_type & 0xffff == flags
    // printf( "i_start:       [%d]:[0x%x]\n", file_node.i_start );
    // printf( "i_length:      [%d]:[0x%x]\n", file_node.i_length );


    printf( "[0x%x] | /%s -> l:[%d] a:[0x%x] a:[%d] \n", (file_node.i_type >> 16), file_node.i_name, file_node.i_name_l, addr, addr );

    return 1;
    // ------------------------------------------------------------

}

// ====================================================================
void call_TMX_FS_MK_DIR() {
    TMX_FS_MK_DIR( "---",  "---" );

}

uint8_t TMX_FS_MK_DIR( char path[], char name[] ) {

    // ------------------------------------------------------------
    uint8_t hd_num = 0;
    uint16_t MAIN_BASE_DATA_0[ 256 ] = { 0 };
    uint32_t ONE_LBA_DATA[ 128 ] = { 0 };
    uint32_t ONE_LBA_DATA_i = 0;

    uint32_t _ADDR_ = 0;

    uint16_t _16bit_H_ = 0, _16bit_L_ = 0;

    uint32_t SIZE_OF_ADDR_SPACE = 1024;
    uint32_t CURR_LBA = 0;


    //printf("mkdir: argv: [%s]\n", cmd_line.argv[1] );

    //printf(" skipping header: [4 x LBAs]\n");
    while ( CURR_LBA +1 < 4 ) { // HEADERS_TTL
        CURR_LBA++;

    }

    uint32_t _get = CURR_LBA+1;

    while ( CURR_LBA < _get ) {

        SET_LBA_READ( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );
        LBA_READ( HD_INFO[ hd_num ].i_drive, (void *)MAIN_BASE_DATA_0 );


        uint16_t i = 0;
        while ( i < 256 ) { // == 256

            _16bit_H_ = MAIN_BASE_DATA_0[ i+1 ]; // BIT-REVERCED
            _16bit_L_ = MAIN_BASE_DATA_0[ i ];   // BIT-REVERCED

            _ADDR_ = ( MAIN_BASE_DATA_0[ i+1 ] << 16 ) | ( MAIN_BASE_DATA_0[ i ] );

            if ( _ADDR_ == 0x00 ) break;

            //printf("\t[0x%x -> 0x%x] >> [0x%x] \n", _16bit_H_, _16bit_L_, _ADDR_ );
            ONE_LBA_DATA[ ONE_LBA_DATA_i ] = _ADDR_;

            i += 2;
            ONE_LBA_DATA_i++;

        }


        CURR_LBA++;

        //break; // just first ADDR-BLOCK: LBA

    }

    //printf("mkdir :: @ [%d]\n", ONE_LBA_DATA_i);
    if ( ONE_LBA_DATA_i >= 128 ) {
        printf("['/'] node MAX-LIMIT of 128 x 32-bit int addr\n");
        return 0;

    }

    // ----------------------------------------------------
    // NOTE:
    CURR_LBA = 3;
    // NOTE:

    uint32_t NODE_LBA_ADDRESS = ( 2 + 2 + 1024 ) + ONE_LBA_DATA_i; // BLK [ NUM ]

    ONE_LBA_DATA[ ONE_LBA_DATA_i ] = NODE_LBA_ADDRESS;

    SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );
    LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ONE_LBA_DATA );

    // ----------------------------------------------------
    CURR_LBA = NODE_LBA_ADDRESS;

    tmx_fs_file_node_t *ppp_file_node;

    ppp_file_node->i_name_l  = strlen( cmd_line.argv[1] );

    for ( uint8_t i=0; i< ppp_file_node->i_name_l; i++) {
        ppp_file_node->i_name[i] = cmd_line.argv[1][i];
        ppp_file_node->i_name[i+1] = 0;

    }


    ppp_file_node->i_parent  = 0xabcd;
    ppp_file_node->i_type    = 0x8000;
    ppp_file_node->i_start   = 0x1000;
    ppp_file_node->i_length  = 0x0001;


    SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );
    LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)ppp_file_node );


    //char *test_data = (char *)cmd_line.argv[1];
    //LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)test_data );


    /*
    uint16_t _16[256] = { 0 };

    uint16_t d = 0x1000;
    _16[0] = d++;
    _16[1] = d++;
    _16[2] = d++;
    _16[3] = d++;
    _16[4] = d++;
    _16[5] = d++;

    LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)_16 );
    */


    //printf("mkdir :: END\n");


    // ------------------------------------------------------------
    return path[0]+name[0];
    // ------------------------------------------------------------
}

// ====================================================================
uint8_t TMX_FS_CAT( char path[], char name[] ) {
    // ------------------------------------------------------------
    return path[0]+name[0];
    // ------------------------------------------------------------
}


// ====================================================================
uint8_t read_sec() {

    if ( cmd_line.argc < 1 ) {
        printf("NO LBA WAS SELECTED: [%d]\n", cmd_line.argc );
        return 1;
    }

    int32_t CURR_LBA = atoi( cmd_line.argv[1] );

    if ( CURR_LBA < 0 ) {
        printf("LBA MUST BE uint32_t: [%d]\n", CURR_LBA );
        return 1;

    }

    uint8_t DATA_READ[512] = { 0 };
    uint8_t hd_num = 0;

    printf("READ: [%d]\n", CURR_LBA); usleep(100);
    SET_LBA_READ( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );
    LBA_READ( HD_INFO[ hd_num ].i_drive, (void *)DATA_READ );

    uint8_t i=0;
    while ( i < 255 ) {
        printf("%c", DATA_READ[i++] );

    }
    printf(" \n" );

    return 0;

}
// ====================================================================
uint8_t write_sec() {

    if ( cmd_line.argc < 1 ) {
        printf("NO LBA WAS SELECTED: [%d]\n", cmd_line.argc );
        return 1;
    }

    int32_t CURR_LBA = atoi( cmd_line.argv[1] );

    if ( CURR_LBA < 0 ) {
        printf("LBA MUST BE uint32_t: [%d]\n", CURR_LBA );
        return 1;

    }

    uint8_t DATA_WRITE[512] = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat.";

    uint8_t hd_num = 0;

    printf("WRITE: [%d]\n", CURR_LBA); usleep(100);
    SET_LBA_WRITE( HD_INFO[ hd_num ].i_drive, HD_INFO[ hd_num ].i_mode, CURR_LBA, 1 );
    LBA_WRITE( HD_INFO[ hd_num ].i_drive , (void *)DATA_WRITE );


    return 0;

}

