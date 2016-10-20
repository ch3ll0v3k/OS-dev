#ifndef _KERNEL_TMX_FS_H_
#define _KERNEL_TMX_FS_H_

#include "kernel.h"
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

// ====================================================================
#endif
