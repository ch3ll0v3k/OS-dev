#ifndef _KERNEL_EXT2_H_
#define _KERNEL_EXT2_H_

#include "kernel.h"

// ====================================================================
// [/usr/include/linux/ext2_fs.h]
// http://www.virtualblueness.net/Ext2fs-overview/Ext2fs-overview-0.1-1.html
// ====================================================================
#define EXT2_SUPERBLOCK_OFFSET 1024

void kernel_init_ext2_fs_support();


uint8_t ext2_superblock_info();
uint8_t ext2_descriptor_table_info();
//uint8_t ext2_inode_info();
uint8_t ext2_inode_info( uint32_t inode );
uint8_t ext2_directory();

// ATA_0 [master]
int get_0_master_info();

// ATA_0 [slave]
int get_0_slave_info();

// ATA_1 [master]
int get_1_master_info();

// ATA_1 [slave]
int get_1_slave_info();

int hddetect( uint16_t drive, uint32_t mode);
void get_root_dir();

uint8_t get_inode_type( uint32_t i_mode );
uint8_t get_dir_entry_type( uint32_t i_mode );
// ====================================================================
// Special inode numbers

#define EXT2_BAD_INO 1              // Bad blocks inode */
#define EXT2_ROOT_IN 2              // Root inode */
#define EXT2_ACL_IDX_IN 3           // ACL inode */
#define EXT2_ACL_DATA_INO 4         // ACL inode */
#define EXT2_BOOT_LOADER_INO 5      // Boot loader inode */
#define EXT2_UNDEL_DIR_INO 6        // Undelete directory inode */

// i_mode
#define EXT2_I_MODE_LINK    0xA000  // – символическая ссылка;
#define EXT2_I_MODE_FILE    0x8000  // – обычный файл;
#define EXT2_I_MODE_BLOCK   0x6000  // – файл блочного устройства;
#define EXT2_I_MODE_DIR     0x4000  // – каталог;
#define EXT2_I_MODE_DEV     0x2000  // – файл символьного устройства;
#define EXT2_I_MODE_FIFO    0x1000  // – канал FIFO.



// ====================================================================
// UUID=a42bd67a-3727-4e11-a459-c3f5eb9c8a71, volume name "root-fs"

typedef struct {

    uint32_t ID;                            // [UUID_STRUCT] 64
    char *UUID;                             // [UUID_STRUCT] 64
    char *volume_name;                      // [UUID_STRUCT] 64
} __attribute__((packed)) dev_info_t;

dev_info_t dev_info;

// ====================================================================
// .../__docs__/FS/ext/The%20Second%20Extended%20File%20System.html#SUPERBLOCK
// 3.1. Superblock
// The superblock is always located at byte offset 1024 from the beginning of the file,
// block device or partition formatted with Ext2 and later variants (Ext3, Ext4).
// Its structure is mostly constant from Ext2 to Ext3 and Ext4 with only some minor changes.

typedef struct {
    uint32_t     s_inodes_count;             // [SUPER] Inodes count
    uint32_t     s_blocks_count;             // [SUPER] Blocks count
    uint32_t     s_r_blocks_count;           // [SUPER] Reserved blocks count
    uint32_t     s_free_blocks_count;        // [SUPER] Free blocks count
    uint32_t     s_free_inodes_count;        // [SUPER] Free inodes count
    uint32_t     s_first_data_block;         // [SUPER] First Data Block
    uint32_t     s_log_block_size;           // [SUPER] Block size
    uint32_t     s_log_frag_size;            // [SUPER] Fragment size
    uint32_t     s_blocks_per_group;         // [SUPER] *** Blocks per group ***
    uint32_t     s_frags_per_group;          // [SUPER] *** Fragments per group ***
    uint32_t     s_inodes_per_group;         // [SUPER] *** Inodes per group ***
    uint32_t     s_mtime;                    // [SUPER] Mount time
    uint32_t     s_wtime;                    // [SUPER] Write time
    uint16_t     s_mnt_count;                // [SUPER] Mount count
    uint16_t     s_max_mnt_count;            // [SUPER] Maximal mount count
    uint16_t     s_magic;                    // [SUPER] Magic signature
    uint16_t     s_state;                    // [SUPER] File system state
    uint16_t     s_errors;                   // [SUPER] Behaviour when detecting errors
    uint16_t     s_minor_rev_level;          // [SUPER] minor revision level
    uint32_t     s_lastcheck;                // [SUPER] time of last check
    uint32_t     s_checkinterval;            // [SUPER] max. time between checks
    uint32_t     s_creator_os;               // [SUPER] OS
    uint32_t     s_rev_level;                // [SUPER] Revision level
    uint16_t     s_def_resuid;               // [SUPER] Default uid for reserved blocks
    uint16_t     s_def_resgid;               // [SUPER] Default gid for reserved blocks

    // These fields are for EXT2_DYNAMIC_REV superblocks only.
    // Note: the difference between the compatible feature set and
    // the incompatible feature set is that if there is a bit set
    // in the incompatible feature set that the kernel doesn't
    // know about, it should refuse to mount the filesystem.
    // e2fsck's requirements are more strict; if it doesn't know
    // about a feature in either the compatible or incompatible
    // feature set, it must abort and not try to meddle with
    // things it doesn't understand...

    uint32_t     s_first_ino;                // [SUPER] First non-reserved inode
    uint16_t     s_inode_size;               // [SUPER] size of inode structure
    uint16_t     s_block_group_nr;           // [SUPER] block group # of this superblock
    uint32_t     s_feature_compat;           // [SUPER] compatible feature set
    uint32_t     s_feature_incompat;         // [SUPER] incompatible feature set
    uint32_t     s_feature_ro_compat;        // [SUPER] readonly-compatible feature set
    uint8_t      s_uuid[16];                 // [SUPER] 128-bit uuid for volume
    uint8_t      s_volume_name[16];          // [SUPER] volume name
    uint8_t      s_last_mounted[64];         // [SUPER] directory where last mounted
    uint32_t     s_algorithm_usage_bitmap;   // [SUPER] For compression

    // Performance hints.  Directory preallocation should only
    // happen if the EXT2_COMPAT_PREALLOC flag is on.

    uint8_t     s_prealloc_blocks;          // [SUPER] Nr of blocks to try to preallocate*/
    uint8_t     s_prealloc_dir_blocks;      // [SUPER] Nr to preallocate for dirs
    uint16_t    s_padding1;                 // [SUPER] padding 2byte

    // Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.

    uint8_t     s_journal_uuid[16];         // [SUPER] uuid of journal superblock
    uint32_t    s_journal_inum;             // [SUPER] inode number of journal file
    uint32_t    s_journal_dev;              // [SUPER] device number of journal file
    uint32_t    s_last_orphan;              // [SUPER] start of list of inodes to delete
    uint32_t    s_hash_seed[4];             // [SUPER] HTREE hash seed
    uint8_t     s_def_hash_version;         // [SUPER] Default hash version to use
    uint8_t     s_reserved_char_pad;        // [SUPER]
    uint16_t    s_reserved_word_pad;        // [SUPER]
    uint32_t    s_default_mount_opts;       // [SUPER]
    uint32_t    s_first_meta_bg;            // [SUPER] First metablock block group
    uint32_t    s_reserved[190];            // [SUPER] Padding to the end of the block

} __attribute__((packed)) ext2_superblock_t; // SIZE [264 + 760 == 1024 ]

ext2_superblock_t ext2_super;

// ====================================================================
// http://www.virtualblueness.net/Ext2fs-overview/Ext2fs-overview-0.1-7.html

// (32 + 32 + 32 + 16 + 16 + 16 + 16 + (32 * 3)) / 8 == 32 byte
typedef struct {

    uint32_t    bg_block_bitmap;            // [GROUP_DESC] Blocks bitmap block
    uint32_t    bg_inode_bitmap;            // [GROUP_DESC] Inodes bitmap block
    uint32_t    bg_inode_table;             // [GROUP_DESC] Inodes table block
    uint16_t    bg_free_blocks_count;       // [GROUP_DESC] Free blocks count
    uint16_t    bg_free_inodes_count;       // [GROUP_DESC] Free inodes count
    uint16_t    bg_used_dirs_count;         // [GROUP_DESC] Directories count
    uint16_t    bg_pad;                     // [GROUP_DESC] __PADDING__
    uint32_t    bg_reserved[3];

} __attribute__ ((packed)) ext2_group_desc_t;

ext2_group_desc_t ext2_group;

// ====================================================================
// http://www.virtualblueness.net/Ext2fs-overview/Ext2fs-overview-0.1-10.html

#define EXT2_DIRECT_BLOCKS 15

typedef struct {
    uint16_t   i_mode;                      // [I_NODE] File mode */
    uint16_t   i_uid;                       // [I_NODE] Owner Uid */
    uint32_t   i_size;                      // [I_NODE] Size in bytes */
    uint32_t   i_atime;                     // [I_NODE] Access time */
    uint32_t   i_ctime;                     // [I_NODE] Creation time */
    uint32_t   i_mtime;                     // [I_NODE] Modification time */
    uint32_t   i_dtime;                     // [I_NODE] Deletion Time */
    uint16_t   i_gid;                       // [I_NODE] Group Id */
    uint16_t   i_links_count;               // [I_NODE] Links count */
    uint32_t   i_blocks;                    // [I_NODE] Blocks count */
    uint32_t   i_flags;                     // [I_NODE] File flags */
    union {
        struct {
            uint32_t  l_i_reserved1;
        } linux1;
        struct {
            uint32_t  h_i_translator;
        } hurd1;
        struct {
            uint32_t  m_i_reserved1;
        } masix1;
    } osd1;                                 // [I_NODE] OS dependent 1 */
    uint32_t   i_block[ EXT2_DIRECT_BLOCKS ];      // [I_NODE] Pointers to blocks */
    uint32_t   i_version;                   // [I_NODE] File version (for NFS) */
    uint32_t   i_file_acl;                  // [I_NODE] File ACL */
    uint32_t   i_dir_acl;                   // [I_NODE] Directory ACL */
    uint32_t   i_faddr;                     // [I_NODE] Fragment address */
    union {
        struct {
            uint8_t    l_i_frag;            // [I_NODE] Fragment number */
            uint8_t    l_i_fsize;           // [I_NODE] Fragment size */
            uint16_t   i_pad1;
            uint32_t   l_i_reserved2[2];
        } linux2;
        struct {
            uint8_t    h_i_frag;            // [I_NODE] Fragment number */
            uint8_t    h_i_fsize;           // [I_NODE] Fragment size */
            uint16_t   h_i_mode_high;
            uint16_t   h_i_uid_high;
            uint16_t   h_i_gid_high;
            uint32_t   h_i_author;
        } hurd2;
        struct {
            uint8_t    m_i_frag;            // [I_NODE] Fragment number */
            uint8_t    m_i_fsize;           // [I_NODE] Fragment size */
            uint16_t   m_pad1;
            uint32_t   m_i_reserved2[2];
        } masix2;
    } osd2;                                 // [I_NODE] OS dependent 2 */

    //} __attribute__ ((packed)) ext2_inode_t;
} ext2_inode_t;

ext2_inode_t ext2_inode;

// ====================================================================
// http://www.virtualblueness.net/Ext2fs-overview/Ext2fs-overview-0.1-11.html
// file:///m-sys/prog-proj/OS-dev/__docs__/FS/ext/The%20Second%20Extended%20File%20System.html
// Directories must be 4byte aligned, and cannot extend between multiple blocks on the disk

#define EXT2_NAME_LEN 255

typedef struct { // [REVISION >= 0.5]

    // --------------------------------------------------------------
    uint32_t    inode;                      // [DIR] Inode number
    uint16_t    rec_len;                    // [DIR] Directory entry length

    uint8_t     name_len;                   // [DIR] Name length
    uint8_t     file_type;                  // [DIR] file type
    int8_t      name[ 255 ];                // [DIR] File name
    // --------------------------------------------------------------


} __attribute__ ((packed)) ext2_dir_entry_t;

// (32-bit)4 + (16-bit)2 + (1-bit)1 + (1-bit)1 + (8 * 255 -bit) 255

// 4 + 2 + 1 + 1 + 255 == 263

// ====================================================================
// structure returned by ATA_CMD_IDENT, as per ANSI ATA2 rev.2f spec

typedef struct {
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

    // 15:9 reserved
    // 8    security level 1:max 0:high
    // 7:6  reserved
    // 5    enhanced erase
    // 4    expire
    // 3    frozen
    // 2    locked
    // 1    en/disabled
    // 0    capability

    uint16_t    csfo;                       // [ATA_IDENT] current set features options

    // 15:4 reserved
    // 3    auto reassign
    // 2    reverting
    // 1    read-look-ahead
    // 0    write cache

    uint16_t    words130_155[26];           // [ATA_IDENT] reserved vendor words 130-155
    uint16_t    word156;                    // [ATA_IDENT] +2
    uint16_t    words157_159[3];            // [ATA_IDENT] reserved vendor words 157-159
    uint16_t    words160_162[3];            // [ATA_IDENT] reserved words 160-162
    uint16_t    cf_advanced_caps;           // [ATA_IDENT] +2
    uint16_t    words164_255[92];           // [ATA_IDENT] reserved words 164-255

} __attribute__((packed)) hd_drive_id_t;


hd_drive_id_t hd_drive_id;


// ====================================================================
#endif
