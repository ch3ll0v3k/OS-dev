#include "h/kernel.h"
// ====================================================================
uint32_t BASE_OFFSET;
uint32_t block_size;
uint32_t frag_size;

// ====================================================================
void get_root_dir() {
    if ( ext2_inode_info(2) ) ext2_directory();

}
/* ====================================================================
ls -i ../mnt/
    32769 bin
    12289 boot
    8193 etc
    15 os.iso
    12 root_0.file
    13 root_1.file
    14 root_2.file
    16385 usr

ls -i ../mnt/boot/
    12293 grub
    12295 kernel.bin

ls -i ../mnt/boot/grub/
    12294 grub.cfg

-----------------------------------------
stat ../mnt/
  File: `../mnt/'
  Size: 1024        Blocks: 2          IO Block: 1024   directory
Device: 700h/1792d  Inode: 2           Links: 7

-----------------------------------------
stat ../mnt/os.iso
  File: `../mnt/os.iso'
  Size: 1996800     Blocks: 3918       IO Block: 1024   regular file
Device: 700h/1792d  Inode: 15          Links: 1

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
stat ../mnt/boot/
  File: `../mnt/boot/'
  Size: 1024        Blocks: 2          IO Block: 1024   directory
Device: 700h/1792d  Inode: 12289       Links: 3

-----------------------------------------
stat ../mnt/boot/kernel.bin
  File: `../mnt/boot/kernel.bin'
  Size: 41388       Blocks: 84         IO Block: 1024   regular file
Device: 700h/1792d  Inode: 12295       Links: 1

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
stat ../mnt/boot/grub/
  File: `../mnt/boot/grub/'
  Size: 1024        Blocks: 2          IO Block: 1024   directory
Device: 700h/1792d  Inode: 12293       Links: 2

-----------------------------------------
stat ../mnt/boot/grub/grub.cfg
  File: `../mnt/boot/grub/grub.cfg'
  Size: 530         Blocks: 2          IO Block: 1024   regular file
Device: 700h/1792d  Inode: 12294       Links: 1

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


// ==================================================================== */

void kernel_init_ext2_fs_support() {

    // ------------------------------------------------------------
    // kernel_log_info(" [EXT2] [KERNEL_INIT_EXT2_FS_SUPPORT]\n");

    ATA_0_STATUS = 0;
    ATA_1_STATUS = 0;

    BASE_OFFSET = 1024;
    block_size = 0;
    frag_size = 0;

    if ( ext2_superblock_info() ) {
        //printf(" [0:1]");
        if ( ext2_descriptor_table_info() ) {
            //printf(" [1:1]");

            uint8_t ROOT_INODE = 2;

            if ( ext2_inode_info( ROOT_INODE ) ) {

                //ext2_directory();

            } else {

                kernel_log_error(" [2:0] [I_NODE ERROR]");
                return;
            }

        } else {
            kernel_log_error(" [1:0] [G-D-T ERROR]");
            return;
        }
    } else {
        kernel_log_error(" [0:0] [SUPER-BLOCK ERROR]");
        return;
    }

    // ------------------------------------------------------------
    // printf(" ---------------------------------- \n");
    // kernel_log_ok(" [EXT2] [KERNEL_INIT_EXT2_FS_SUPPORT]: [DONE]\n");
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

    uint32_t CC = 0;
    while ( (inb( ATA_0 + 7) & 0x80) ) {

        if ( CC++ == 1000 ) {
            k_term_hist_off();
            printf("ATA_WAIT ... \n");
            k_term_hist_on();
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
dumpe2fs ./256M_fs.ext2 | grep superblock
Primary ['SUPERBLOCK'] at 1,        ['GROUP DESCRIPTORS'] at 2-2
Backup  ['SUPERBLOCK'] at 8193,     ['GROUP DESCRIPTORS'] at 8194-8194
Backup  ['SUPERBLOCK'] at 24577,    ['GROUP DESCRIPTORS'] at 24578-24578
Backup  ['SUPERBLOCK'] at 40961,    ['GROUP DESCRIPTORS'] at 40962-40962
Backup  ['SUPERBLOCK'] at 57345,    ['GROUP DESCRIPTORS'] at 57346-57346
Backup  ['SUPERBLOCK'] at 73729,    ['GROUP DESCRIPTORS'] at 73730-73730
Backup  ['SUPERBLOCK'] at 204801,   ['GROUP DESCRIPTORS'] at 204802-204802
Backup  ['SUPERBLOCK'] at 221185,   ['GROUP DESCRIPTORS'] at 221186-221186
*/
// ====================================================================
uint8_t ext2_superblock_info() {

    // -----------------------------------------------------------
    uint8_t DEBUG = 0;

    // ------------------------------------------------------------
    uint32_t ATA_LBA_SUPER_BLOCK = 0;
    uint32_t BLK_P_GROUP = 8192;

    uint32_t X_X = 2; // == (OFF_X_byte / BYTE_IN_SEC);
    //uint32_t LBA_MUL = 2;

    ATA_LBA_SUPER_BLOCK = X_X + (( BLK_P_GROUP * 0 ) );             // PRIMARY [SUPER-BLOCK] [+1 GDT]
    //ATA_LBA_SUPER_BLOCK = X_X + (( BLK_P_GROUP * 1 ) * LBA_MUL);    // BACKUP  [SUPER-BLOCK] [+1 GDT]
    //ATA_LBA_SUPER_BLOCK = X_X + (( BLK_P_GROUP * 2 ) * LBA_MUL);    // [BLOCK BITMAP] [+1 INODE BITMAP] [+1 INODE TABLE]



    // -----------------------------------------------------------
    ATA_LBA = ATA_LBA_SUPER_BLOCK;
    ATA_SEC_CNT = 2;

    uint32_t BLOCK_NR = ATA_LBA / 2;

    set_LBA_SEC_CNT( ATA_LBA, ATA_SEC_CNT );

    // -----------------------------------------------------------
    uint32_t cnt_read = 512;
    uint16_t DATA[ cnt_read ]; // ext2_superblock_t

    for ( uint32_t i = 0; i < cnt_read; i++) {

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );

        DATA[i] = inw( ATA_0 );

    }

    ext2_super = *((ext2_superblock_t*) DATA);


    // MAGIC == 0XEF53 // 0x53 0xEF at positions 1080–1081
    if ( ext2_super.s_magic != 0XEF53 ) { // [61267] [0xef53]

        kernel_log_error(" [EXT2] [IS-NOT-S_BLOCK]\n" );
        printf("[NOT S_BLOCK] s_magic: [0x%x] != [0XEF53] | ATA_LBA:[%d] | BLOCK_NR:[%d] |\n\n", ext2_super.s_magic, ATA_LBA, BLOCK_NR );

        return 0;
    }


    // sudo tune2fs -l ../rootfs.ext2

    block_size = 1024 << ext2_super.s_log_block_size;
    frag_size = (ext2_super.s_log_frag_size > 0) ? (1024 << ext2_super.s_log_frag_size) : (1024 >> -ext2_super.s_log_frag_size);

    // -----------------------------------------------------------
    if ( DEBUG ) {

        printf(" --------------------------------------------------------------\n");
        printf(" [EXT2] [SUPER-BLOCK] [hd0] ATA_LBA: [%d] BLOCK_NR: [%d]:\n", ATA_LBA, BLOCK_NR);
        printf(" [EXT2] Fragment size           : [%d]\n", frag_size );
        //printf(" [EXT2] MAGIC                   : [0x%x]\n", ext2_super.s_magic );
        printf(" [EXT2] Volume name             : [%s]\n", ext2_super.s_volume_name );
        //printf(" [EXT2] UUID                    : [%s]\n", ext2_super.s_uuid );
        printf(" [EXT2] Inodes count            : [%d]\n", ext2_super.s_inodes_count );
        printf(" [EXT2] Blocks count            : [%d]\n", ext2_super.s_blocks_count  );
        printf(" [EXT2] Reserved blocks count   : [%d]\n", ext2_super.s_r_blocks_count  );      /* reserved blocks count */
        printf(" [EXT2] Free blocks count       : [%d]\n", ext2_super.s_free_blocks_count  );
        printf(" [EXT2] Free inodes count       : [%d]\n", ext2_super.s_free_inodes_count  );
        printf(" [EXT2] First data block        : [%d]\n", ext2_super.s_first_data_block  );
        printf(" [EXT2] Blocks per group        : [%d]\n", ext2_super.s_blocks_per_group  );
        printf(" [EXT2] Block size [k]          : [%d]\n", block_size  );
        printf(" [EXT2] Inodes per group        : [%d]\n", ext2_super.s_inodes_per_group  );
        //printf(" [EXT2] Creator OS              : [%d]\n", ext2_super.s_creator_os  );
        //printf(" [EXT2] First non-reserved inode: [%d]\n", ext2_super.s_first_ino  );   /* first non-reserved inode */
        //printf(" [EXT2] Size of inode structure : [%d]\n", ext2_super.s_inode_size  );

        usleep(250);
    }

    return 1;
    // -----------------------------------------------------------
    /*
    [EXT2] [SUPER-BLOCK] [hd0] ATA_LBA: [2] ATA_SEC_CNT: [2]:
    [EXT2] Fragment size           : [1024]     <<<
    [EXT2] MAGIC                   : [0xEF53]
    [EXT2] Volume name             : [root-fs]
    [EXT2] UUID                    : [id]
    [EXT2] Inodes count            : [8192]     <<<
    [EXT2] Blocks count            : [32768]
    [EXT2] Reserved blocks count   : [1638]
    [EXT2] Free blocks count       : [28890]
    [EXT2] Free inodes count       : [8038]
    [EXT2] First data block        : [1]
    [EXT2] Blocks per group        : [8192]     <<<
    [EXT2] Block size [k]          : [1024]     <<<
    [EXT2] Inodes per group        : [2048]
    [EXT2] Creator OS              : [0]
    [EXT2] First non-reserved inode: [11]
    [EXT2] Size of inode structure : [128]

    */
    // -----------------------------------------------------------

}

// ====================================================================
uint8_t ext2_descriptor_table_info() {

    // ------------------------------------------------------------
    uint8_t DEBUG = 0;
    // -----------------------------------------------------------

    uint32_t ATA_LBA_GDT_BLOCK = 0;
    uint32_t BLK_P_GROUP = 8192;

    uint32_t X_X = 2; // == (OFF_X_byte / BYTE_IN_SEC);
    //uint32_t LBA_MUL = 2;

    ATA_LBA_GDT_BLOCK = X_X + (( BLK_P_GROUP * 0 ) ) + 2;             // PRIMARY [SUPER-BLOCK] [+1 GDT]
    //ATA_LBA_GDT_BLOCK = X_X + (( BLK_P_GROUP * 1 ) * LBA_MUL) + 2;  // BACKUP  [SUPER-BLOCK] [+1 GDT]



    // -----------------------------------------------------------
    ATA_LBA = ATA_LBA_GDT_BLOCK;

    ATA_SEC_CNT = 1;

    set_LBA_SEC_CNT( ATA_LBA, ATA_SEC_CNT );
    // ------------------------------------------------------------
    uint32_t cnt_read = 256;
    uint16_t DATA[ cnt_read ];

    for ( uint32_t i = 0; i < cnt_read; i++) {

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );

        DATA[i] = inw( ATA_0 );
    }

    ext2_group = *( (ext2_group_desc_t *) DATA);

    // -----------------------------------------------------------
    if ( DEBUG ) {

        printf(" --------------------------------------------------------------\n");
        printf(" [EXT2] [GROUP_DESC] [hd0] ATA_LBA: [%d] ATA_SEC_CNT: [%d]:\n\n", ATA_LBA, ATA_SEC_CNT);
        printf(" [EXT2] bg_block_bitmap         : [%d]\n", ext2_group.bg_block_bitmap );
        printf(" [EXT2] bg_inode_bitmap         : [%d]\n", ext2_group.bg_inode_bitmap );
        printf(" [EXT2] bg_inode_table          : [%d]\n", ext2_group.bg_inode_table );
        printf(" [EXT2] bg_free_blocks_count    : [%d]\n", ext2_group.bg_free_blocks_count );
        printf(" [EXT2] bg_free_inodes_count    : [%d]\n", ext2_group.bg_free_inodes_count );
        printf(" [EXT2] bg_used_dirs_count      : [%d]\n", ext2_group.bg_used_dirs_count );
        printf(" [EXT2] bg_pad                  : [%d]\n", ext2_group.bg_pad );
        printf(" [EXT2] bg_reserved[3]          : [ RESERVED ]\n" );
        usleep(250);

    }

    // -----------------------------------------------------------
    return 1;
    // ------------------------------------------------------------
    /*
    [EXT2] [GROUP_DESC] [hd0] ATA_LBA: [4] ATA_SEC_CNT: [1]:

    [EXT2] bg_block_bitmap         : [130]
    [EXT2] bg_inode_bitmap         : [131]
    [EXT2] bg_inode_table          : [132]
    [EXT2] bg_free_blocks_count    : [5816]
    [EXT2] bg_free_inodes_count    : [2027]
    [EXT2] bg_used_dirs_count      : [2]
    [EXT2] bg_pad                  : [4]
    [EXT2] bg_reserved[3]          : [ RESERVED ]
    */
    // ------------------------------------------------------------

}

// ====================================================================
uint8_t ext2_root_inode_info() {

    // -----------------------------------------------------------
    uint8_t DEBUG = 0;
    // -----------------------------------------------------------
    uint8_t root_inode = 2;

    uint32_t OFFSET =
        ( BASE_OFFSET+ (ext2_group.bg_inode_table - 1) * block_size )
        +
        (root_inode -1) * sizeof(ext2_inode);

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


    ext2_root_inode = *( (ext2_inode_t *) DATA );

    if ( DEBUG ) {

        printf(" --------------------------------------------------------------\n");
        printf(" [EXT2] [I_DONE] [hd0] ATA_LBA: [%d] ATA_SEC_CNT: [%d]:\n\n", ATA_LBA, ATA_SEC_CNT);
        printf(" >>> OFFSET: [%d] byte | LBA_OFFSET: [%d]\n", OFFSET, ATA_LBA );
        printf(" [EXT2] inode.i_mode:           [%c]\n", get_inode_type( ext2_root_inode.i_mode ) ); // [I_NODE] File mode

        if ( (ext2_root_inode.i_mode & 0xF000 ) == EXT2_I_MODE_DIR ) {   //0x4000  // –

            printf(" [EXT2] ** DIRECTORY**          [%d]\n", root_inode);
            printf(" [EXT2] inode.i_uid:            [%d]\n", ext2_root_inode.i_uid ); // [I_NODE] Owner Uid
            printf(" [EXT2] inode.i_size:           [%d]\n", ext2_root_inode.i_size ); // [I_NODE] Size in bytes
            printf(" [EXT2] inode.i_atime:          [%d]\n", ext2_root_inode.i_atime ); // [I_NODE] Access time
            printf(" [EXT2] inode.i_ctime:          [%d]\n", ext2_root_inode.i_ctime ); // [I_NODE] Creation time
            printf(" [EXT2] inode.i_mtime:          [%d]\n", ext2_root_inode.i_mtime ); // [I_NODE] Modification time
            printf(" [EXT2] inode.i_dtime:          [%d]\n", ext2_root_inode.i_dtime ); // [I_NODE] Deletion Time
            printf(" [EXT2] inode.i_gid:            [%d]\n", ext2_root_inode.i_gid ); // [I_NODE] Group Id
            printf(" [EXT2] inode.i_links_count:    [%d]\n", ext2_root_inode.i_links_count ); // [I_NODE] Links count
            printf(" [EXT2] inode.i_blocks:         [%d]\n", ext2_root_inode.i_blocks ); // [I_NODE] Blocks count
            printf(" [EXT2] inode.i_flags:          [%d]\n", ext2_root_inode.i_flags ); // [I_NODE] File flags

        } else {
            printf(" [EXT2] ** NOT-DIRECTORY**      [%d]\n", root_inode);

        }
    }

    // -----------------------------------------------------------
    if ( (ext2_root_inode.i_mode & 0xF000 ) == EXT2_I_MODE_DIR )
        return 1;

    return 0;

    // -----------------------------------------------------------


}
// ====================================================================

uint8_t ext2_inode_info( uint32_t inode_no ) {

    // -----------------------------------------------------------
    uint8_t DEBUG = 0;
    // -----------------------------------------------------------
    //uint32_t OFFSET = ( BASE_OFFSET + (ext2_group.bg_inode_table - 1) * block_size )
    uint32_t OFFSET = ( BASE_OFFSET + (ext2_group.bg_inode_table - 1) * block_size )
                      +
                      (inode_no -1) * sizeof(ext2_inode_t);

    ATA_LBA = OFFSET / 512; // 135296 / 512 == 264.25

    ATA_SEC_CNT = 1;

    set_LBA_SEC_CNT( ATA_LBA, ATA_SEC_CNT );

    uint32_t cnt_read = 256;
    uint16_t DATA[ cnt_read ];

    uint32_t ii=0;
    for ( uint32_t i = 0; i < cnt_read; i++) {

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );


        //DATA[ii++] = inw( ATA_0 );

        /* */
        DATA[ii] = inw( ATA_0 );

        if ( i > 63) { // 264.25 -> QUART OF 512
            ii++;
        }
        /* */

    }


    ext2_inode = *( (ext2_inode_t *) DATA );

    if ( (ext2_inode.i_mode & 0xF000 ) == EXT2_I_MODE_DIR ) {
        //printf(" [%d] --------------------- \n", inode_no);
        //ext2_directory();
        //usleep(20);
    }

    if ( DEBUG ) {

        printf(" --------------------------------------------------------------\n");
        // [EXT2] [I_DONE] [hd0] ATA_LBA: [264] ATA_SEC_CNT: [1]:
        printf(" [EXT2] [I_DONE] [hd0] ATA_LBA: [%d] ATA_SEC_CNT: [%d]:\n\n", ATA_LBA, ATA_SEC_CNT);
        // >>> OFFSET: [135296] byte | LBA_OFFSET: [264] <<< 264 * 2 528
        printf(" >>> OFFSET: [%d] byte | LBA_OFFSET: [%d]\n", OFFSET, ATA_LBA );

        // [EXT2] inode.i_mode:           [D]
        printf(" [EXT2] inode.i_mode:           [%c]\n", get_inode_type( ext2_inode.i_mode ) ); // [I_NODE] File mode

        if ( (ext2_inode.i_mode & 0xF000 ) == EXT2_I_MODE_DIR ) {   //0x4000  // –

            // [EXT2] ** DIRECTORY**          [2]
            printf(" [EXT2] ** DIRECTORY**          [%d]\n", inode_no);
            // [EXT2] inode.i_uid:            [1000]
            printf(" [EXT2] inode.i_uid:            [%d]\n", ext2_inode.i_uid );
            // [EXT2] inode.i_size:           [1024]
            printf(" [EXT2] inode.i_size:           [%d]\n", ext2_inode.i_size );
            // [EXT2] inode.i_atime:          [1475401530]
            printf(" [EXT2] inode.i_atime:          [%d]\n", ext2_inode.i_atime );
            // [EXT2] inode.i_ctime:          [1475401573]
            printf(" [EXT2] inode.i_ctime:          [%d]\n", ext2_inode.i_ctime );
            // [EXT2] inode.i_mtime:          [1475401573]
            printf(" [EXT2] inode.i_mtime:          [%d]\n", ext2_inode.i_mtime );
            // [EXT2] inode.i_dtime:          [0]
            printf(" [EXT2] inode.i_dtime:          [%d]\n", ext2_inode.i_dtime );
            // [EXT2] inode.i_gid:            [1000]
            printf(" [EXT2] inode.i_gid:            [%d]\n", ext2_inode.i_gid );
            // [EXT2] inode.i_links_count:    [19]
            printf(" [EXT2] inode.i_links_count:    [%d]\n", ext2_inode.i_links_count );
            // [EXT2] inode.i_blocks:         [2]
            printf(" [EXT2] inode.i_blocks:         [%d]\n", ext2_inode.i_blocks );
            // [EXT2] inode.i_flags:          [0]
            printf(" [EXT2] inode.i_flags:          [%d]\n", ext2_inode.i_flags );

        } else {
            printf(" [EXT2] ** NOT-DIRECTORY**      [%d]\n", inode_no);

        }
    }

    return 1;
    // -----------------------------------------------------------

}

// ====================================================================
uint8_t ext2_directory( ) {

    // -----------------------------------------------------------
    //uint8_t DEBUG = 0;

    // -----------------------------------------------------------
    uint32_t OFFSET = (BASE_OFFSET+ (ext2_inode.i_block[0] - 1) * block_size);

    ATA_LBA = OFFSET / 512; // _calc "397312 / 512 == 776.0
    ATA_SEC_CNT = 1; // inode == 1024

    set_LBA_SEC_CNT( ATA_LBA, ATA_SEC_CNT );
    uint32_t cnt_read = sizeof(ext2_dir_entry_t); // 263 byte == sizeof(ext2_dir_entry_t)
    uint16_t DATA_DIR[ cnt_read ];

    for ( uint32_t i = 0; i < cnt_read; i++) {

        if ( i % 64 == 0 )
            while ( inb( ATA_0 + 7) & 0x80 ) ATA_WAIT_400ns( ATA_0_CTRL );

        DATA_DIR[ i ] = inw( ATA_0 );

    }
    /////////////////////////////////////////////////////////////////////////////////////////////

    uint32_t size = 0;
    //uint32_t show_hidden = 0, max_display = 2, curr_dir_num = 0;

    ext2_dir_entry_t *entry = (ext2_dir_entry_t *) DATA_DIR;

    while ( (size < ext2_inode.i_size) ) { // && entry->inode ) {

        if ( entry->rec_len < 1 ) break;

        entry->name[ entry->name_len ] = 0;

        /* *********** FILTER *************
        if ( !show_hidden && entry->name[0] == '.' ) {

        } else {
            if ( curr_dir_num++ < max_display ) {
                printf("[%c] i[%d] /%s\n", get_dir_entry_type( entry->file_type ), entry->inode, entry->name);
                usleep( 20 );

            } else {
                printf(" ..... >\n");

                return 1;
            }
        }
        *********** FILTER ************* */

        printf("[%c] i[%d] /%s\n", get_dir_entry_type( entry->file_type ), entry->inode, entry->name);

        entry = (ext2_dir_entry_t *) ((void *)entry + entry->rec_len);
        size += entry->rec_len;

    }

    //usleep(200);
    return 1;
    // -----------------------------------------------------------
    /*
    term#:/ ls
    [D] i[2] /.
    [D] i[2] /..
    [D] i[12289] /boot
    [D] i[34817] /.Trash-1000
    [F] i[12] /root_0.file
    [D] i[8193] /etc
    [D] i[16385] /usr
    [D] i[32769] /bin
    [F] i[15] /os.iso
    [F] i[13] /root_1.file
    [F] i[14] /root_2.file
    */
    // -----------------------------------------------------------

}

// ====================================================================
uint8_t get_dir_entry_type( uint32_t i_mode ) {

    // -----------------------------------------------------------
    if ( i_mode == 0x00 ) return 'U'; // ET2_FT_UNKNOWN   0   Unknown File Type
    if ( i_mode == 0x01 ) return 'F'; // ET2_FT_REG_FILE  1   Regular File
    if ( i_mode == 0x02 ) return 'D'; // ET2_FT_DIR       2   Directory File
    if ( i_mode == 0x03 ) return 'C'; // ET2_FT_CHRDEV    3   Character Device
    if ( i_mode == 0x04 ) return 'B'; // ET2_FT_BLKDEV    4   Block Device
    if ( i_mode == 0x05 ) return 'O'; // ET2_FT_FIFO      5   Buffer File
    if ( i_mode == 0x06 ) return 'S'; // ET2_FT_SOCK      6   Socket File
    if ( i_mode == 0x07 ) return 'L'; // ET2_FT_SYMLINK   7   Symbolic Link

    return 'N';
    // -----------------------------------------------------------
}

// ====================================================================
uint8_t get_inode_type( uint32_t i_mode ) {

    // -----------------------------------------------------------
    if ( (i_mode & 0xF000 ) == EXT2_I_MODE_LINK )   return 'L'; // 0xA000
    if ( (i_mode & 0xF000 ) == EXT2_I_MODE_FILE )   return 'F'; // 0x8000
    if ( (i_mode & 0xF000 ) == EXT2_I_MODE_BLOCK )  return 'B'; // 0x6000
    if ( (i_mode & 0xF000 ) == EXT2_I_MODE_DEV )    return 'E'; // 0x2000
    if ( (i_mode & 0xF000 ) == EXT2_I_MODE_FIFO )   return 'O'; // 0x1000
    if ( (i_mode & 0xF000 ) == EXT2_I_MODE_DIR )    return 'D'; // 0x4000

    return 'N';
    // -----------------------------------------------------------

}

// ====================================================================
// ATA_0 [master]
void get_0_master_info() { hddetect( ATA_0, 0xA0);}

// ATA_0 [slave]
void get_0_slave_info() { hddetect( ATA_0, 0xB0); }

// ATA_1 [master]
void get_1_master_info() { hddetect( ATA_1, 0xA0);}

// ATA_1 [slave]
void get_1_slave_info() { hddetect( ATA_1, 0xB0); }


int hddetect( uint16_t drive, uint32_t mode) {

    printf("CONTROLLER: [0x%x] READY: [0x%x] \n", drive + 7, inb(drive + 7) );

    outb(drive + 6, mode); /* masterselect 0xa0 master */
    outb(drive + 7, 0xEC); /* ask for data */

    while ( inb(drive + 7) != 0x58) { // check if data is ready

        printf("CONTROLLER: [0x%x] READY: [0x%x] \n", drive + 7, inb(drive + 7) );
        __asm__ __volatile__ ("nop");

    }

    uint16_t data[256];

    for ( uint16_t i = 0; i<256; i++) { // Read the data
        data[i] = inw(drive);
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

    return 1;
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
