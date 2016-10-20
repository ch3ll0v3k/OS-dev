#ifndef _KERNEL_ATA_UTILS_H_
#define _KERNEL_ATA_UTILS_H_

#include "kernel.h"
// ====================================================================

uint8_t hdd_info( int32_t drive, int32_t mode);

uint8_t init_ata_utils();
uint8_t get_hd_info( uint8_t i);

// ====================================================================
typedef struct {

    int8_t i_name[111];
    uint8_t i_name_l;
    uint32_t i_parent;
    uint32_t i_type;
    // i_type >> 16 == type
    // i_type & 0xffff == flags

    uint32_t i_start;
    uint32_t i_length;
    uint8_t rest[512-128];

} __attribute__((packed)) tmx_fs_file_node_t;


// ====================================================================
typedef struct {
    uint8_t available;      //
    uint32_t i_drive;       // (int) ATA_0 || ATA_1
    uint32_t i_mode;        // (int) ATA_MASTER || ATA_SLAVE

    uint32_t L_cyls;        // = HD.cur_cyls; //  logical cylinders
    uint32_t L_heads;       // = HD.cur_heads; //  logical heads
    uint32_t L_sectors;     // = HD.cur_sectors; //  logical sectors per track
    uint32_t L_capacity0;   // = HD.cur_capacity0; //  logical total sectors on drive

    uint32_t P_cyls;        // = HD.cyls; //  physical cylinders
    uint32_t P_heads;       // = HD.heads; //  physical heads
    uint32_t P_sectors;     // = HD.sectors; //  physical sectors per track
    uint32_t P_lba_capacity; // = HD.lba_capacity; // total number of sectors
    uint32_t size_MiB;      // = HD.lba_capacity * 512 / 1024 / 1024;
    uint32_t size_GiB;      // = HD.lba_capacity * 512 / 1024 / 1024 / 1024;

    int8_t s_drive[6];     // (char[] ) ATA_0 || ATA_1
    int8_t s_mode[6];      // (char[] ) ATA_MASTER || ATA_SLAVE

    int8_t capability[8];  // = (cp == 3) ? "IORDYsu" : (cp == 2) ? "IORDYsw" : (cp == 1) ? "LBA" : "DMA";
    int8_t tDMA[4];        // = (HD.tDMA == 2) ? "FAST" : (HD.tDMA == 1) ? "MEDD" : "SLOW"; // 0=slow, 1=medium, 2=fast
    //model:              [%s] \n", HD.model );
    //serial_no:          [%d] [0x%x] \n", HD.serial_no, HD.serial_no );

} __attribute__ ((packed)) HD_INFO_t;


HD_INFO_t HD_INFO[16];

uint16_t HDDS_TTL_CNT;
uint8_t ATA_UTILS_INITED;

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
