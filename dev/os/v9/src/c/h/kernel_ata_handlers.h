#ifndef _KERNEL_ATA_HANDLERS_H
#define _KERNEL_ATA_HANDLERS_H

#include "kernel.h"

// ====================================================================
// Master / slave.
#define ATA_MASTER 0xE0
#define ATA_SLAVE 0xF0

#define ATA_0 0x1F0
#define ATA_1 0x170

// ATA commands.
#define ATA_READ 0x24
#define ATA_WRITE 0x34

#define ATA_0_CTRL 0x3F6
#define ATA_1_CTRL 0x376

// Status register bits
#define ATA_STAT_ERR            0x01 // (1 << 0) 00000001 => Error       
#define ATA_STAT_INDEX          0x02 // (1 << 1) 00000010 => Vendor specific 
#define ATA_STAT_CORR           0x04 // (1 << 2) 00000100 => Corrected Data Error    
#define ATA_STAT_DRQ            0x08 // (1 << 3) 00001000 => Data Request (ready)    
#define ATA_STAT_SEEK           0x10 // (1 << 4) 00010000 => Device Seek Complete    
#define ATA_STAT_FAULT          0x20 // (1 << 5) 00100000 => Device Fault        
#define ATA_STAT_READY          0x40 // (1 << 6) 01000000 => Device Ready        
#define ATA_STAT_BUSY           0x80 // (1 << 7) 10000000 => Device Busy     

// Device / Head Register Bits
#define ATA_DEVICE(x)   ((x & 1)<<4)
#define ATA_LBA_MODE         0xE0    // 224

// ATA Commands (only mandatory commands listed here)
#define ATA_CMD_READ            0x20     // Read Sectors (with retries)
#define ATA_CMD_READN           0x21     // Read Sectors ( no  retries)
#define ATA_CMD_WRITE           0x30     // Write Sectores (with retries
#define ATA_CMD_WRITEN          0x31     // Write Sectors  ( no  retries
#define ATA_CMD_VRFY            0x40     // Read Verify  (with retries)
#define ATA_CMD_VRFYN           0x41     // Read verify  ( no  retries)
#define ATA_CMD_SEEK            0x70     // Seek            
// GOOGLE
// 0x71 to 0x7F ATA1 through ATA2  SEEK
#define ATA_CMD_DIAG            0x90     // Execute Device Diagnostic
#define ATA_CMD_INIT            0x91     // Initialize Device Parameters
#define ATA_CMD_RD_MULT         0xC4     // Read Multiple   
#define ATA_CMD_WR_MULT         0xC5     // Write Multiple  
#define ATA_CMD_SETMULT         0xC6     // Set Multiple Mode   
#define ATA_CMD_RD_DMA          0xC8     // Read DMA (with retries)
#define ATA_CMD_RD_DMAN         0xC9     // Read DMS ( no  retries)
#define ATA_CMD_WR_DMA          0xCA     // Write DMA (with retries)
#define ATA_CMD_WR_DMAN         0xCB     // Write DMA ( no  retires)
#define ATA_CMD_IDENT           0xEC     // Identify Device 
#define ATA_CMD_SETF            0xEF     // Set Features        
#define ATA_CMD_CHK_PWR         0xE5     // Check Power Mode    

// SPS
#define ATA_CMD_READ_EXT        0x24     // Read Sectors (with retries) with 48bit addressing
#define ATA_CMD_WRITE_EXT       0x34     // Write Sectores (with retries) with 48bit addressing
#define ATA_CMD_VRFY_EXT        0x42     // Read Verify (with retries)  with 48bit addressing

// ATAPI Commands
#define ATAPI_CMD_IDENT         0xA1  // Identify AT Atachment Packed Interface Device
#define ATAPI_CMD_PACKET        0xA0  // Packed Command

#define ATAPI_CMD_INQUIRY       0x12
#define ATAPI_CMD_REQ_SENSE     0x03
#define ATAPI_CMD_READ_CAP      0x25
#define ATAPI_CMD_START_STOP    0x1B
#define ATAPI_CMD_READ_12       0xA8


// ====================================================================
uint16_t ATA_0_STATUS;
uint16_t ATA_1_STATUS;

uint32_t ATA_LBA;
uint32_t ATA_SEC_CNT;

void ATA_WAIT_400ns( uint16_t ATA_CTRL );
void ATA_GET_READY( uint16_t ATA, uint16_t ATA_CTRL );



void ATA_0_HANDLER( REG *reg );
void ATA_1_HANDLER( REG *reg );
void INSTALL_ATA_HANDLERS();






// ====================================================================
#endif
