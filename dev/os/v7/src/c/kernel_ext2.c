#include "h/kernel.h"
// ====================================================================
uint32_t BASE_OFFSET;
uint32_t block_size;

uint8_t GLOBAL_BIT;

// ====================================================================
//int hddetect(int driveselect, int master) { /* drive value 0x1f0 */
int hddetect( ) { /* drive value 0x1f0 */
    int i, j, err = 1;

    int driveselect = 0x1f0;
    int master = 1;

    uint32_t masterselect;
    char model[256];
    char serial[256];

    if (master > 0) {
        masterselect = 0xa0;
    } else {
        masterselect = 0xb0;
    }

    uint32_t readbuff   = 0x1f0; //driveselect;
    uint32_t drivectl   = 0x1f0 + 6; //driveselect + 6;
    uint32_t controller = 0x1f0 + 7; //driveselect + 7;


    while ( inb(0x1f0 + 7) != 0x58) { // check if data is ready
        __asm__ __volatile__ ("nop");

    }

    printf("CONTROLLER: [0x%x] READY: [0x%x] \n", controller, inb(controller) );

    outb(0x1f0 + 6, 0xa0); /* masterselect 0xa0 master */
    outb(0x1f0 + 7, 0xEC); /* ask for data */

    while ( inb(0x1f0 + 7) != 0x58) { // check if data is ready
        __asm__ __volatile__ ("nop");

    }

    uint16_t data[256];

    for (i = 0; i<256; i++) { // Read the data
        data[i] = inw(0x1f0);
    }


    printf("--------------------------------------------------------------\n");
    hd_drive_id_t HD = *((hd_drive_id_t*) data);

    printf("vendor[0-6]: [%d %d %d %d %d %d] \n", HD.vendor0, HD.vendor1, HD.vendor2, HD.vendor3, HD.vendor4, HD.vendor5, HD.vendor6 ); // [ATA_IDENT] bits 0:DMA 1:LBA 2:IORDYsw 3:IORDYsu

    uint8_t cp = HD.capability;

    printf("capability:     [%s] \n", (cp == 3) ? "IORDYsu" : (cp == 2) ? "IORDYsw" : (cp == 1) ? "LBA" : "DMA" ); // bits 0:DMA 1:LBA 2:IORDYsw 3:IORDYsu

    printf("model:          [%s] \n", HD.model );
    printf("serial_no:      [%s] \n", HD.serial_no, HD.serial_no );
    printf("tDMA:           [%s] \n", (HD.tDMA == 2) ? "FAST" : (HD.tDMA == 1) ? "MED" : "SLOW" ); // 0=slow, 1=medium, 2=fast


    printf("cur_cyls:       [%d] [0x%x] \n", HD.cur_cyls, HD.cur_cyls ); //  logical cylinders
    printf("cur_heads:      [%d] [0x%x] \n", HD.cur_heads, HD.cur_heads ); //  logical heads
    printf("cur_sectors:    [%d] [0x%x] \n", HD.cur_sectors, HD.cur_sectors ); //  logical sectors per track
    printf("cur_capacity0:  [%d] [0x%x] \n", HD.cur_capacity0, HD.cur_capacity0 ); //  logical total sectors on drive



    return err;
}

// ====================================================================
//void ide_poll(uint16_t ATA_0) {
void ide_poll() {

    uint32_t ATA_REG_ALTSTATUS = 0x0C;
    uint32_t ATA_REG_STATUS = 0x07;

    for (int i=0; i< 4; i++)
        inb(ATA_0 + ATA_REG_ALTSTATUS);

retry:;
    uint8_t status = inb(ATA_0 + ATA_REG_STATUS);
    printf("testing for BSY\n");
    if (status & ATA_STAT_BUSY) goto retry;
    printf("BSY cleared\n");
retry2: status = inb(ATA_0 + ATA_REG_STATUS);
    if (status & ATA_STAT_ERR) {
        kernel_log_error("ERR set, device failure!\n");
    }
    printf("testing for DRQ\n");
    if (!(status & ATA_STAT_DRQ)) goto retry2;
    printf("DRQ set, ready to PIO!\n");
    return;
}

// ====================================================================

void kernel_init_ext2_fs_support() {

    // ------------------------------------------------------------
    printf(" [EXT2] kernel_init_ext2_fs_support\n");

    ATA_0_STATUS = 0;
    ATA_1_STATUS = 0;

    BASE_OFFSET = 1024;
    block_size = 0;

    // printf(" sizeof(hd_drive_id) [%d]\n", sizeof(hd_drive_id) );

    // hddetect();
    // printf(" sizeof(ext2_inode_t) [%d]\n", sizeof(ext2_inode_t) ); // 128
    // return;




    /* */
    if ( ext2_superblock_info() ) {
        if ( ext2_descriptor_table_info() ) {
            uint32_t i = 1;

            k_term_clear();
            while ( i < 120) {

                if ( ext2_inode_info(i++) ) ext2_directory();
            }




        }
    }
    /**/


    /*

    GLOBAL_BIT = 1;
    uint32_t i = 1;

    while ( i < 64000 && GLOBAL_BIT) {

        ext2_inode_info(i++);
    }


    printf("FOUND AT [%d]\n", --i );


    */
    // 21114


    //return;

    // ------------------------------------------------------------
    // Assume you have a sectorcount byte and a 28 bit LBA value.
    // A sectorcount of 0 means 256 sectors = 128K.

    // [0] => [256] => [128 k]
    // [1] => [512] => [256 k]
    // [2] => [768] => [384 k]
    // [3] => [1024] => [512 k]

    // [4] => [1280] => [640 k]
    // [5] => [1536] => [768 k]
    // [6] => [1792] => [896 k]
    // [7] => [2048] => [1024 k]

    // [8] => [2304] => [1152 k]
    // [9] => [2560] => [1280 k]
    // [10] => [2816] => [1408 k]
    // [11] => [3072] => [1536 k]

    // [12] => [3328] => [1664 k]
    // [13] => [3584] => [1792 k]
    // [14] => [3840] => [1920 k]
    // [15] => [4096] => [2048 k]


    // ------------------------------------------------------------

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

    while ( (inb( ATA_0 + 7) & 0x80) ) {
        printf("ATA_WAIT ... \n");
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
uint8_t ext2_superblock_info() {

    // ------------------------------------------------------------
    ATA_LBA = 2;
    ATA_SEC_CNT = 2;

    set_LBA_SEC_CNT( ATA_LBA, ATA_SEC_CNT );

    // -----------------------------------------------------------
    uint32_t cnt_read = 512;
    uint16_t DATA[ cnt_read ]; // ext2_superblock_t

    for ( uint32_t i = 0; i < cnt_read; i++) {

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );

        DATA[i] = inw( ATA_0 );

    }

    ext2_superblock = *((ext2_superblock_t*) DATA);

    // MAGIC == 0XEF53 // 0x53 0xEF at positions 1080–1081
    if ( ext2_superblock.s_magic != 0XEF53 ) { // [61267] [0xef53]

        printf(" [EXT2] [IS-NOT-S_BLOCK] s_magic: [0x%x] != [0XEF53]\n\n", ext2_superblock.s_magic );
        return 0;
    }


    // sudo tune2fs -l ../rootfs.ext2

    printf(" --------------------------------------------------------------\n");
    printf(" [EXT2] [SUPER-BLOCK] [hd0] ATA_LBA: [%d] ATA_SEC_CNT: [%d]:\n\n", ATA_LBA, ATA_SEC_CNT);

    block_size = 1024 << ext2_superblock.s_log_block_size;

    printf(" [EXT2] MAGIC                   : [0x%x]\n", ext2_superblock.s_magic );
    printf(" [EXT2] Volume name             : [%s]\n", ext2_superblock.s_volume_name );
    //printf(" [EXT2] UUID                    : [%s]\n", ext2_superblock.s_uuid );
    //printf(" [EXT2] Inodes count            : [%d]\n", ext2_superblock.s_inodes_count );
    //printf(" [EXT2] Blocks count            : [%d]\n", ext2_superblock.s_blocks_count  );
    //printf(" [EXT2] Reserved blocks count   : [%d]\n", ext2_superblock.s_r_blocks_count  );      /* reserved blocks count */
    //printf(" [EXT2] Free blocks count       : [%d]\n", ext2_superblock.s_free_blocks_count  );
    //printf(" [EXT2] Free inodes count       : [%d]\n", ext2_superblock.s_free_inodes_count  );
    //printf(" [EXT2] First data block        : [%d]\n", ext2_superblock.s_first_data_block  );
    //printf(" [EXT2] Blocks per group        : [%d]\n", ext2_superblock.s_blocks_per_group  );
    //printf(" [EXT2] Block size [k]          : [%d]\n", block_size  );
    //printf(" [EXT2] Inodes per group        : [%d]\n", ext2_superblock.s_inodes_per_group  );
    //printf(" [EXT2] Creator OS              : [%d]\n", ext2_superblock.s_creator_os  );
    //printf(" [EXT2] First non-reserved inode: [%d]\n", ext2_superblock.s_first_ino  );   /* first non-reserved inode */
    //printf(" [EXT2] Size of inode structure : [%d]\n", ext2_superblock.s_inode_size  );
    //printf(" [EXT2] ext2_superblock_info    : [END]\n\n");

    usleep(250);


    return 1;
    // -----------------------------------------------------------

}

// ====================================================================
uint8_t ext2_descriptor_table_info() {

    // ------------------------------------------------------------
    ATA_LBA = 4;
    ATA_SEC_CNT = 1;

    set_LBA_SEC_CNT( ATA_LBA, ATA_SEC_CNT );

    printf(" --------------------------------------------------------------\n");
    printf(" [EXT2] [GROUP_DESC] [hd0] ATA_LBA: [%d] ATA_SEC_CNT: [%d]:\n\n", ATA_LBA, ATA_SEC_CNT);

    uint32_t cnt_read = 256;
    uint16_t DATA[ cnt_read ];

    for ( uint32_t i = 0; i < cnt_read; i++) {

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );

        DATA[i] = inw( ATA_0 );
    }

    ext2_group_desc = *( (ext2_group_desc_t *) DATA);

    printf(" [EXT2] bg_block_bitmap         : [%d]\n", ext2_group_desc.bg_block_bitmap );
    printf(" [EXT2] bg_inode_bitmap         : [%d]\n", ext2_group_desc.bg_inode_bitmap );
    printf(" [EXT2] bg_inode_table          : [%d]\n", ext2_group_desc.bg_inode_table );
    printf(" [EXT2] bg_free_blocks_count    : [%d]\n", ext2_group_desc.bg_free_blocks_count );
    printf(" [EXT2] bg_free_inodes_count    : [%d]\n", ext2_group_desc.bg_free_inodes_count );
    //printf(" [EXT2] bg_used_dirs_count      : [%d]\n", ext2_group_desc.bg_used_dirs_count );
    //printf(" [EXT2] bg_pad                  : [%d]\n", ext2_group_desc.bg_pad );
    //printf(" [EXT2] bg_reserved[3]          : [ RESERVED : 3 ]\n" );

    usleep(250);

    return 1;
    // ------------------------------------------------------------

}

// ====================================================================
uint8_t ext2_inode_info( uint32_t this_inode_no ) {

    // -----------------------------------------------------------
    uint32_t inode_no = this_inode_no;

    uint32_t OFFSET = ( BASE_OFFSET+ (ext2_group_desc.bg_inode_table - 1) * block_size )
                      +
                      (inode_no -1) * sizeof(ext2_inode);

    ATA_LBA = OFFSET / 512; // 135296 / 512 == 264.25

    ATA_SEC_CNT = 1;

    set_LBA_SEC_CNT( ATA_LBA, ATA_SEC_CNT );

    uint32_t cnt_read = 256;
    uint16_t DATA[ cnt_read ];

    uint32_t ii=0;
    for ( uint32_t i = 0; i < cnt_read; i++) {

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );

        DATA[ii] = inw( ATA_0 );

        if ( i > 63) { // 264.25 -> QUART OF 512
            ii++;
        }

    }


    ext2_inode = *( (ext2_inode_t *) DATA );

    // ( ext2_inode.i_mode >> 12 ) == 0x4  OR (ext2_inode.i_mode & 0xF000 ) == 0x4000
    if ( (ext2_inode.i_mode >> 12) == 0x4 ) { // directory

        printf(" --------------------------------------------------------------\n");
        printf(" [EXT2] [I_DONE] [hd0] ATA_LBA: [%d] ATA_SEC_CNT: [%d]:\n\n", ATA_LBA, ATA_SEC_CNT);
        printf(" >>> OFFSET: [%d] byte | LBA_OFFSET: [%d]\n", OFFSET, ATA_LBA );

        printf(" [EXT2] inode.i_mode:           [%d]\n", ext2_inode.i_mode ); // [I_NODE] File mode
        printf(" [EXT2] inode.i_uid:            [%d]\n", ext2_inode.i_uid ); // [I_NODE] Owner Uid
        printf(" [EXT2] inode.i_size:           [%d]\n", ext2_inode.i_size ); // [I_NODE] Size in bytes
        printf(" [EXT2] inode.i_atime:          [%d]\n", ext2_inode.i_atime ); // [I_NODE] Access time
        printf(" [EXT2] inode.i_ctime:          [%d]\n", ext2_inode.i_ctime ); // [I_NODE] Creation time
        printf(" [EXT2] inode.i_mtime:          [%d]\n", ext2_inode.i_mtime ); // [I_NODE] Modification time
        printf(" [EXT2] inode.i_dtime:          [%d]\n", ext2_inode.i_dtime ); // [I_NODE] Deletion Time
        printf(" [EXT2] inode.i_gid:            [%d]\n", ext2_inode.i_gid ); // [I_NODE] Group Id
        printf(" [EXT2] inode.i_links_count:    [%d]\n", ext2_inode.i_links_count ); // [I_NODE] Links count
        printf(" [EXT2] inode.i_blocks:         [%d]\n", ext2_inode.i_blocks ); // [I_NODE] Blocks count
        printf(" [EXT2] inode.i_flags:          [%d]\n", ext2_inode.i_flags ); // [I_NODE] File flags

    }


    usleep(250);
    return 1;
    // -----------------------------------------------------------

}

// ====================================================================
uint8_t ext2_directory( ) {

    // -----------------------------------------------------------

    // kernel_ext2.h | >>> uint32_t i_block[ FIXME >> EXT2_DIRECT_BLOCKS ]; // [I_NODE] Pointers to blocks */
    uint32_t OFFSET = (BASE_OFFSET+ (ext2_inode.i_block[0] - 1) * block_size);

    ATA_LBA = OFFSET / 512; // _calc "397312 / 512 == 776.0
    ATA_SEC_CNT = 1; // inode == 1024


    printf(" --------------------------------------------------------------\n");
    printf(" [EXT2] [DIR] [hd0] ATA_LBA: [%d] ATA_SEC_CNT: [%d]:\n", ATA_LBA, ATA_SEC_CNT);
    printf( "OFFSET: [%d]\n\n", OFFSET );


    set_LBA_SEC_CNT( ATA_LBA, ATA_SEC_CNT );



    uint32_t cnt_read = 256; // 264 byte ==  sizeof(ext2_dir_entry)
    uint16_t DATA[ cnt_read ];

    uint32_t ii=0;
    for ( uint32_t i = 0; i < cnt_read; i++) {

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );

        DATA[ii++] = inw( ATA_0 );

        //if ( i > 63) { // 264.25 -> QUART OF 512
        //    ii++;
        //}

    }

    // ext2_dir_entry_t entry = *( (ext2_dir_entry_t *) DATA );
    ext2_dir_entry_t *entry = (ext2_dir_entry_t *)DATA ;
    unsigned int size = 0;

    while ((size < ext2_inode.i_size) && entry->inode && entry->rec_len) {

        ext2_dir_entry_t entry_2 = *((ext2_dir_entry_t *)(entry));

        char BF[256];

        uint32_t ii = 0;

        while ( ii < entry_2.name_len ) {
            BF[ ii ] = entry_2.name[ ii ];
            ii++;
        }
        BF[ ii ] = '\0';

        printf("[%d] [%d] [%s] \n", entry_2.inode, entry_2.rec_len, BF );
        // * * printf( " [DIR] inode:      [%d] \n", entry_2.inode );
        // * * printf( " [DIR] rec_len:    [%d] \n", entry_2.rec_len );
        // * * printf( " [DIR] name_len:   [%d] \n", entry_2.name_len );
        // * * printf( " [DIR] name:       [%s] \n", entry_2.name );

        entry = (void *)entry + entry->rec_len;
        size += entry->rec_len;

        usleep(20);


    }



    return 1;
    // -----------------------------------------------------------

}
// ====================================================================
