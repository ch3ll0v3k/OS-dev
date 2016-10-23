#ifndef _KERNEL_TMX_FS_H_
#define _KERNEL_TMX_FS_H_

#include "kernel.h"
// ====================================================================
typedef struct {

    uint8_t i_name[110];            // 110 + 0 == 111
    uint8_t i_name_l;               // 1 byte +112
    uint32_t i_parent;              // 4 byte +116
    uint32_t i_type;                // 4 byte +120
    // i_type >> 16 == type
    // i_type & 0xffff == flags

    uint32_t i_start;               // byte +124
    uint32_t i_length;              // byte +128
    uint8_t rest[512-128];          // +(128 + 386) == 0 == 512

} __attribute__((packed)) tmx_fs_file_node_t;


// ====================================================================
uint8_t SET_LBA_WRITE( uint32_t drive, uint32_t ata_lba_mode, uint32_t lba, uint32_t ata_sec_cnt);
uint8_t LBA_WRITE( uint32_t drive, void *data );

uint8_t SET_LBA_READ( uint32_t drive, uint32_t ata_lba_mode, uint32_t lba, uint32_t ata_sec_cnt);
uint8_t LBA_READ( uint32_t drive, void *data );

uint8_t DRIVE_WAIT( uint32_t drive );

// ----------------------------------------------------------------
uint8_t TMX_FS_FORMAT( uint16_t drive );
void call_TMX_FS_FORMAT();

// ----------------------------------------------------------------
uint8_t TMX_FS_GET_PWD( char path[], char name[] );

uint8_t TMX_FS_LIST_DIR( char path[], char name[] );
void call_TMX_FS_LIST_DIR();


uint8_t TMX_FS_CAT( char path[], char name[] );

uint8_t TMX_FS_RM( char path[], char name[] );
uint8_t TMX_FS_MK_FILE( char path[], char name[] );


uint8_t TMX_FS_MK_DIR( char path[], char name[] );
void call_TMX_FS_MK_DIR();


uint8_t get_file_info( uint32_t addr );

uint8_t read_sec();
uint8_t write_sec();

// ====================================================================
#endif
