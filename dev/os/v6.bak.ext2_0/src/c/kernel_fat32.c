#include "h/kernel.h"
// ====================================================================

/*
// on Primary bus: ctrl->base =0x1F0, ctrl->dev_ctl =0x3F6. REG_CYL_LO=4, REG_CYL_HI=5, REG_DEVSEL=6
int detect_devtype (int slavebit, struct DEVICE *ctrl) {
    ata_soft_reset(ctrl->dev_ctl);      // waits until master drive is ready again
    outb(ctrl->base + REG_DEVSEL, 0xA0 | slavebit<<4);
    inb(ctrl->dev_ctl);         // wait 400ns for drive select to work
    inb(ctrl->dev_ctl);
    inb(ctrl->dev_ctl);
    inb(ctrl->dev_ctl);
    unsigned cl=inb(ctrl->base + REG_CYL_LO);   // get the "signature bytes"
    unsigned ch=inb(ctrl->base + REG_CYL_HI);

    // differentiate ATA, ATAPI, SATA and SATAPI
    if (cl==0x14 && ch==0xEB) return ATADEV_PATAPI;
    if (cl==0x69 && ch==0x96) return ATADEV_SATAPI;
    if (cl==0 && ch == 0) return ATADEV_PATA;
    if (cl==0x3c && ch==0xc3) return ATADEV_SATA;
    return ATADEV_UNKNOWN;
}
*/
// ====================================================================
// PRIMARY-ATA-BUS [0x1F0 - 0x1F7]
// SECONDARY-ATA-BUS [0x170 - 0x177]

// ../FAT32.fs: x86 boot sector, mkdosfs boot message display,
// code offset 0x58, OEM-ID "mkfs.fat", sectors/cluster 8,
// Media descriptor 0xf8, heads 64,
// sectors 1048576 (volumes > 32 MB) , FAT (32 bit),
// sectors/FAT 1022,
// reserved3 0x1800000,
// reserved 0x1,
// serial number 0xe11c768, unlabeled

#define ATA_DEVICE_CONTROL_PORT 0x3F6

#define ATA_0 0x1F0
#define ATA_A 0x170
#define ATA_CTRL 0x3F6

// ATA commands.
#define ATA_READ 0x24
#define ATA_WRITE 0x34

// Master / slave.
#define ATA_MASTER 0xE0
#define ATA_SLAVE 0xF0

// Status byte bits.
#define ATA_SB_ERR 1
#define ATA_SB_DRQ (1 << 3)
#define ATA_SB_SRV (1 << 4)
#define ATA_SB_DF  (1 << 5)
#define ATA_SB_RDY (1 << 6)
#define ATA_SB_BSY (1 << 7)

// Device control / alternate status bits.
#define ATA_DCR_NO_INT 2
#define ATA_DCR_SW_RESET (1 << 2)
#define ATA_DCR_HOB (1 << 7)

// ====================================================================
void ATA_WAIT() {

    // inb( 0x3F6 ); // wait 100 Nano-Seconds
    inb( ATA_CTRL ); inb( ATA_CTRL ); inb( ATA_CTRL ); inb( ATA_CTRL ); inb( ATA_CTRL );

}

// ====================================================================
// PRIMARY-ATA-BUS [0x1F0 - 0x1F7]
// SECONDARY-ATA-BUS [0x170 - 0x177]

int32_t DRIVES[2][2] = {
    { 0x1F0, 0xA0 },
    { 0x170, 0xB0 }
};

// ====================================================================
uint32_t FAT_INFO_DELAY_MS = 200;

uint8_t get_FAT_directory_info( fat_directory_entry_t dir, uint32_t LBA, uint32_t curr_sec ) {

    if ( strlen( (char *)dir.filename ) > 0 ) {

        printf("\n-----------------------------------------\n" );
        k_term_color = K_TERM_L_GREEN;
        printf("FAT DIR: LBA: [%d] | SECTOR: [%d]\n\n", LBA, curr_sec );
        k_term_color = K_TERM_WHITE;

        printf("ATR: [%c] \n", dir.attributes );
        printf("NAM: [%s] \n", dir.filename );
        printf("EXT: [%s] \n", dir.ext );
        printf("SZE: [%d] byte \n", dir.file_size );
        printf("CLH: [0x%x] \n", dir.cluster_high );
        printf("CLL: [0x%x] \n", dir.cluster_low );
        printf("DAT: [%d] \n", dir.time );
        printf("TME: [%d] \n", dir.date );
        printf("UND: [%s] \n", dir.unused );
        usleep(FAT_INFO_DELAY_MS);
        return 1;

    } else {
        //printf("\n-----------------------------------------\n" );
        //k_term_color = K_TERM_L_RED;
        //printf("NOT FAT DIR: [0x%x] | LBA:[%d] SEC:[%d]\n", dir, LBA, curr_sec );
        //k_term_color = K_TERM_WHITE;
        //usleep(FAT_INFO_DELAY_MS);
        return 0;

    }

}

// char filename[8];
// // padded with spaces and not NULL-terminated
// char ext[3];            // 8+3 == 11 byte for the file name
// uint8_t attributes;
// char unused[8];
// uint16_t cluster_high;
// uint16_t time;
// uint16_t date;
// uint16_t cluster_low;
// uint32_t file_size;


// ====================================================================
uint8_t get_FAT_header_info( fat32_header_t header, uint32_t LBA, uint32_t curr_sec ) {

    if ( strlen( (char *)header.label ) > 0 ) {

        printf("\n-----------------------------------------\n" );
        k_term_color = K_TERM_L_BLUE;
        printf("FAT HEADER: LBA: [%d] | SECTOR: [%d]\n\n", LBA, curr_sec );
        k_term_color = K_TERM_WHITE;

        printf("LBL: [%s]\n", header.label );
        printf("SER: [0x%x] \n", header.serial );
        printf("TYP: [%s] \n", header.media_type );
        printf("SIG: [0x%x] \n", header.signature );
        printf("SCC: [%d] \n", header.sectors_per_track );
        printf("NUM: [%d] \n", header.drive_number );
        usleep(FAT_INFO_DELAY_MS);
        return 1;

    } else {
        //printf("\n-----------------------------------------\n" );
        //k_term_color = K_TERM_L_RED;
        //printf("NOT FAT HEADER: [0x%x] | LBA:[%d] SEC:[%d]\n", header, LBA, curr_sec );
        //k_term_color = K_TERM_WHITE;
        //usleep(FAT_INFO_DELAY_MS);
        return 0;
    }


}

/*
//uint8_t jmp[3];
//char oem[8];
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
//char reserved[12];
uint8_t drive_number;
uint8_t nt_flags;
uint8_t signature; //  should contain either 0x28 or 0x29 in order to be a fully-valid FAT32 partition, and serial is supposed to contain a random 32-bit number that can be used to identify different disks
uint32_t serial;
// spaces-padded and not NULL-terminated!
//char label[11];
// spaces-padded and not NULL-terminated; and never trust the contents.
char identifier[8];
char bootcode[420];
uint16_t bootsig;
*/


// ====================================================================
uint32_t TTL_HEADERS = 0;
uint32_t TTL_DIRS = 0;
uint32_t LBA = 0;
uint32_t SEC_C = 1;

void LBA_UP() { LBA++; }
void LBA_DOWN() { LBA--; }
void LBA_GET() { printf("LBA: [%d]\n", LBA ); }

void SEC_UP() { SEC_C++; }
void SEC_DOWN() { SEC_C--; }
void SEC_GET() { printf("SEC_C: [%d]\n", SEC_C ); }



void fat32_test() {

    // LBA = 0, [SEC 0,6] fat32_header_t is available
    // LBA = 1, [SEC 5] fat32_header_t is available

    uint8_t slave = 0;
    uint16_t status = 0;

    // serial number of ../FAST32.fs [0xe11c768]

    // -----------------------------------------------------------
    // outb( ATA_0 + 6, (LBA >> 24) & 0x0F); //bits 24-28 of LBA
    outb( ATA_0 + 6, 0xE0 | (slave << 4) | ((LBA >> 24) & 0x0F));
    // outb( ATA_0 + 1, 0x00);                          // Send a NULL byte to port 0x1F1, if you like (it is ignored and wastes lots of CPU time): outb(0x1F1, 0x00)
    outb( ATA_0 + 2, (uint8_t)(SEC_C) );
    // * outb( ATA_0 + 3, (uint8_t)( LBA ) );
    // * outb( ATA_0 + 4, (uint8_t)( LBA >> 8 ) );
    // * outb( ATA_0 + 5, (uint8_t)( LBA >> 16 ) );
    outb( ATA_0 + 3, (uint8_t)((LBA) & 0xFF) );         // LBA low byte
    outb( ATA_0 + 4, (uint8_t)((LBA >> 8) & 0xFF) );    // LBA mid byte
    outb( ATA_0 + 5, (uint8_t)((LBA >> 16) & 0xFF) );   // LBA high byte
    outb( ATA_0 + 7, 0x20); // ATA_READ == 0x20  == read sectors with retry

    ATA_WAIT();
    // -----------------------------------------------------------

    while (status & 0x80) {
        printf("ATA DELAY\n" ); usleep(100 );
    }


    for ( uint8_t i=0; i<1; i++ ) {

        // status = inb( ATA_0 + 7);
        // if (status & 0x1)  { printf("ERR\n"); } // ERR set
        // if (status & 0x8)  { printf("DRQ\n"); } // DRQ bit set
        // if (status & 0x20) { printf("DF\n");  } // DF set
        // if (status & 0x80) { printf("BSY\n"); } // BSY set
        // if (status & 0x88) { printf("BSY&DRQ\n"); } // BSY and DRQ must be clear

    }


    uint16_t INSW[ 256 ];
    fat32_header_t header;
    fat_directory_entry_t dir;

    uint32_t curr_sec = 0;
    while ( curr_sec <= SEC_C ) {


        for ( int i = 0; i < 256; i++) {

            //insw( ATA_0, INSW[i], 2 );
            INSW[i] = inw( ATA_0 );
            //printf("[%d]  [%c]\n", INSW[i], INSW[i] );
        }


        if ( !get_FAT_header_info( *((fat32_header_t*) INSW), LBA, curr_sec )) {

            if ( !get_FAT_directory_info( *( (fat_directory_entry_t*) INSW), LBA, curr_sec ) ) {
                k_term_color = K_TERM_L_RED;
                printf("LBA: [%d] SEC: [%d]\n", LBA, curr_sec);
                k_term_color = K_TERM_WHITE;

            } else { TTL_DIRS++; }

        } else { TTL_HEADERS++; }

        ATA_WAIT();

        curr_sec++;
    }


    printf("---------------------------------\n");
    k_term_color = K_TERM_L_GREEN;
    printf("TTL_HEADERS: [%d] | TTL_DIRS: [%d]\n", TTL_HEADERS, TTL_DIRS );
    k_term_color = K_TERM_WHITE;


    return;
    // -----------------------------------------------------------


}


void fat_1() { }
void fat_2() { }
void fat_3() { }


void init_fat32( int32_t drive, int32_t args ) {

    printf("init_fat32( int32_t drive, int32_t args )\n" );

}

// ####################################################################################################

/*void init_fat32( int32_t drive, int32_t args ) {

char *str;
int32_t status;

status =  inb( drive ); usleep(10);
printf("DRIVE: [%x] STATUS: [%d] [0x%x] [%s]\n", drive, status, status, (status & 0x88) ? "BUSY" : "READY" );

uint8_t ata = 0;

while ( ata < 10) {

int32_t status =  inb( DRIVES[0][0]+ata );
usleep(10);

str = (status & 0x88) ? "[BUSY]" : "[READY]";

printf("ATA: [%x] status: [%d] [0x%x] | STATUS: [%s]\n", DRIVES[0][0]+ata, status, status, str );
ata++;

}


//return;

//printf("init_fat32( 0x%x, 0x%x ); \n", drive, args);
// drive could be 0x1F7 or 0x170 and args 0xA0 or 0xB0
outb( drive, args );
usleep(10); // ATA [400ns delays]

int32_t info =  inb(DRIVES[0][0]) & (1 << 6);

printf("info: [%d] [0x%x]\n", info, info );


uint8_t is_slave = 0;
// 0xf8 == Media descriptor
// 0x1F0 == primary base
// 0x58 == code offset
uint16_t addr = 0xf000;
uint32_t sector_count = 4096;

uint16_t buff[ 64 * sector_count ];

outb(drive + 1, 0x00);
outb(drive + 2, sector_count);
outb(drive + 3, (uint8_t) ((addr) & 0xFF));
outb(drive + 4, (uint8_t)((addr >> 8) & 0xFF));
outb(drive + 5, (uint8_t)((addr >> 16) & 0xFF));
outb(drive + 6,  0xE0 | (is_slave << 4) | ((addr >> 24) & 0x0F));
outb(drive + 7, _ATA_READ);

k_term_printnl("ENTER: wait");
while (!(inb(drive + 7) & 0x08)) {
usleep(5);
//k_term_printnl("ata usleep 5");
}
k_term_printnl("EXIT: wait");

for (uint32_t i = 0; i < 256; i++) {

uint16_t tmpword = inw(drive);
//printf(" [%c, %c, %c] - ", buff[i], (uint8_t)tmpword, (uint8_t)(tmpword >> 8) );
buff[i * 2] = (uint8_t)tmpword;
//printf(" [%c, %c, %c] - ", buff[i], (uint8_t)tmpword, (uint8_t)(tmpword >> 8) );
buff[i * 2 + 1] = (uint8_t)(tmpword >> 8);
//printf(" [%c, %c, %c] ", buff[i], (uint8_t)tmpword, (uint8_t)(tmpword >> 8) );


//if ( i % 5 == 0) printf(" \n");

//usleep(1);
}


printf("[%s] \n", buff );


for (uint32_t i = 0; i < 256 * sector_count; i++) {

printf(" %c \0", buff[i] );
usleep(1);

//itoa(buff[i], i, 16);

//printf("0x%s ", itoa(buff[i], 0, 16));
}


}
*/

// ====================================================================
