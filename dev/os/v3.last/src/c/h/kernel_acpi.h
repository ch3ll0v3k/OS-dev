#ifndef _KERNEL_ACPI_H_
#define _KERNEL_ACPI_H_

#include "kernel.h"
// ====================================================================

void read_hdd_lba28( char buffer[], uint8_t addr, uint8_t sector_count, uint16_t drive, char is_slave );

// drive could be 0x1F7 or 0x170 and args 0xA0 or 0xB0
uint8_t poll_drive(uint16_t drive, uint8_t arg);






// ====================================================================
#endif