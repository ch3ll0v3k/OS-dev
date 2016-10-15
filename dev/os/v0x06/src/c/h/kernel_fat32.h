#ifndef _KERNEL_FAT32_H_
#define _KERNEL_FAT32_H_

#include "kernel.h"
// ====================================================================
void init_fat32();


void fat32_test();


void ATA_0_HANDLER( REG *reg );
void ATA_1_HANDLER( REG *reg );
void INSTALL_ATA_HANDLERS();


void LBA_UP();
void LBA_DOWN();
void LBA_GET();

void SEC_UP();
void SEC_DOWN();
void SEC_GET();




uint8_t ata_wait_until_not_busy(uint32_t timeout_ms);
void ata_send_command(unsigned char command);
void ata_select_drive(uint8_t drive);
void ata_interrupt_enable(uint8_t enable);
uint8_t ata_read_lba28(uint32_t LBA, uint8_t sectorCount, void* dest);
// ====================================================================
// primbase is 0x1F0
// 0x1FF == 511
// 0x100 == 512

// .../__docs__/FS/fat/OSDev%20notes%206:%20Filesystems%20and%20ELF%20loading%20-%20ethereality.html
// http://ethv.net/workshops/osdev/notes/notes-6

// [GIT] /m-sys/prog-proj/OS-dev/__docs__/FS$/osdev-refcode/

// ====================================================================
// total number of sectors allocated to the FATs is [sectors_per_fat_large] * [fat_count].
//the filesystem content begins at sector [reserved_sectors] + [sectors_per_fat_large] * [fat_count]
typedef struct {
    uint8_t jmp[3];
    // zero-padded and not necessarily NULL-terminated!
    char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t fat_count;
    uint16_t directory_entry_count;
    uint16_t sector_count_small;
    uint8_t media_type;
    uint16_t sectors_per_fat_small;
    uint16_t sectors_per_track;
    uint16_t head_count;
    uint32_t hidden_sector_count;
    uint32_t sector_count_large;
    // extended attributes
    uint32_t sectors_per_fat_large;
    uint16_t fat_flags;
    uint16_t version;
    uint32_t root_cluster;
    uint16_t fsinfo_sector;
    uint16_t backup_boot_sector;
    char reserved[12];
    uint8_t drive_number;
    uint8_t nt_flags;
    uint8_t signature; //  should contain either 0x28 or 0x29 in order to be a fully-valid FAT32 partition, and serial is supposed to contain a random 32-bit number that can be used to identify different disks
    uint32_t serial;
    // spaces-padded and not NULL-terminated!
    char label[11];
    // spaces-padded and not NULL-terminated; and never trust the contents.
    char identifier[8];
    char bootcode[420];
    uint16_t bootsig;
} __attribute__((packed)) fat32_header_t;

fat32_header_t fat32_header;


// -----------------------------------------------------------------
// The clusters begin immediately after the FATs; and so the filesystem content begins at sector
// [reserved_sectors] + [sectors_per_fat_large] * [fat_count].

// The first two cluster indices are special, so you can find cluster number [i] at sector
// [reserved_sectors] + [sectors_per_fat_large] * [fat_count] + [sectors_per_cluster] * (i-2).
// The content of a cluster is pretty obvious for a file; it's the file contents . . . however, for a directory,
// the cluster contents follow a specific format.
// In particular, each is 32 bytes long and has the following format:

// ----------------
// The first character of filename has a specific format. In particular,
// if it is a NULL character (0x0), then this is the last directory entry in the cluster that should be processed.

// If it's 0xE5, then it should be skipped (it's available for future re-use)
// If it's 0x05, then the first character should actually be a 0xE5.
// filenames shorter than 8.3 should be padded with spaces as appropriate.

// There are [6 ATTRIBUTE BITS].
// >>> [0] "r-only"  bit; if set, the file should not be modified.
// >>> [1] "hidden"  bit; if set then the file should not show up.
// >>> [2] "system"  bit. file is a part of the operating system and really, really, shouldn't be modified.
// >>> [3] "volume"  bit; it's used for a couple of reasons, but nothing that we're interested in for the moment.
// >>> [4] "dir"     bit; this entry is a directory instead of a file.
// >>> [5] "archive" bit, is supposed to be set as soon as a file is created or modified, hence letting a backup (archival) program know which files have changed since the last copy was made. (Assuming, of course, that the program clears the archival bits after the copies are made. Otherwise, it's a waste of a perfectly good bit.)


// ----------------

typedef struct  {
    // padded with spaces and not NULL-terminated
    char filename[8];
    // padded with spaces and not NULL-terminated
    char ext[3];            // 8+3 == 11 byte for the file name
    uint8_t attributes;
    char unused[8];
    uint16_t cluster_high;
    uint16_t time;
    uint16_t date;
    uint16_t cluster_low;
    uint32_t file_size;

} __attribute__((packed)) fat_directory_entry_t;



// ====================================================================

#endif