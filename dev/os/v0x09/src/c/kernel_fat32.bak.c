#include "h/kernel.h"
// ====================================================================
#define _ATA_READ 0x24
#define _ATA_WRITE 0x34


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

// ATA commands.
#define ATA_READ 0x24
#define ATA_WRITE 0x34

// Master / slave.
#define ATA_DRIVE_MASTER 0xE0
#define ATA_DRIVE_SLAVE 0xF0

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

int32_t DRIVES[2][2] = {
    { 0x1F0, 0xA0 },
    { 0x170, 0xB0 }
};


void fat_0() {


    if (ata_read_lba28(1, 1, (void*)0x00140000)) printf("success\n");

    /*
    uint8_t ata = 0;
    while ( ata < 10 ) {

        printf("----------------------------------------------\n");
        printf("ATA: [0x%x]: \n", DRIVES[0][0] + ata );
        init_fat32( DRIVES[0][0]+ata, DRIVES[0][1] );
        printf("  \n" );
        sleep(1);
        ata++;
    }
    */


}

void fat_1() {

    uint8_t ata = 0;
    while ( ata < 10 ) {

        printf("----------------------------------------------\n");
        printf("ATA: [0x%x]: \n", DRIVES[0][0] + ata );
        init_fat32( DRIVES[1][0]+ata, DRIVES[1][1] );
        printf("  \n" );
        sleep(1);
        ata++;
    }

}

void fat_2() { }
void fat_3() { }


void init_fat32( int32_t drive, int32_t args ) {

    printf("init_fat32( int32_t drive, int32_t args )\n" );

}

// ####################################################################################################


uint8_t ata_wait_until_not_busy(uint32_t timeout_ms) {
    uint8_t status = inb(0x1F7);

    //uint32_t startTick = get_tick_count();

    while (status & 0x88) { //BSY and DRQ must be clear
        //if (get_tick_count() > startTick + timeout_ms)
        //    return 0;
        printf(" ******** (status & 0x88): waiting\n");
        usleep(10);
        status = inb(0x1F7);
    }

    return 1;
}

void ata_send_command(unsigned char command) {
    //printf("ata_send_command\n");
    outb(0x1F7, command);
}

void ata_select_drive(uint8_t drive) {
    //printf("ata_select_drive\n");
    uint8_t buf = inb(0x1F6);
    if (drive)
        buf |= 0x10; //set bit 4

    buf = 0;

    outb(0x1F6, buf);
}

void ata_interrupt_enable(uint8_t enable) {
    //printf("ata_interrupt_enable\n");
    uint8_t buf = 0;
    if (enable)
        buf |= 2;

    outb(0x3F6, buf);
}

uint8_t ata_read_lba28(uint32_t LBA, uint8_t sectorCount, void* dest) {

    printf("call : ata_wait_until_not_busy(1000)\n");
    if (!ata_wait_until_not_busy(1000))
        return 0;

    //printf("outb()\n");
    outb(0x1F2, sectorCount); //sector count
    outb(0x1F3, LBA & 0xFF); //LBA low byte
    outb(0x1F4, (LBA >> 8) & 0xFF); //LBA mid byte
    outb(0x1F5, (LBA >> 16) & 0xFF); //LBA high byte
    outb(0x1F6, (LBA >> 24) & 0x0F); //bits 24-28 of LBA

    ata_select_drive(0);

    ata_interrupt_enable(0);
    ata_send_command(0x20); //read sectors with retry

    uint8_t status = 0;

    for (unsigned int i=0; i<4; i++) {
        status = inb(0x1F7);
        if (!(status & 0x80)) { //BSY bit clear
            if (status & 0x8) //DRQ bit set
                goto data_ready;
        }
    }

    printf("while [0]\n");
    while (1) {
        status = inb(0x1F7);
        if (status & 0x80) { //BSY set
            // * printf(" ******** (status & 0x80)\n");
            continue;

        } else {
            // * printf(" ******** (status & 0x80): break\n");
            break;
        }

        if (status & 0x1) { //ERR set
            // * printf(" ******** (status & 0x1)\n");
            return 0;

        }

        if (status & 0x20) { //DF set
            // * printf(" ******** (status & 0x20)\n");
            return 0;
        }
    }


data_ready: ;

    //printf("data_ready\n");
    uint16_t* p = (uint16_t*)dest;

    uint32_t TTL = 256;

    uint16_t out[TTL];
    out[0] = '0';

    for (unsigned int i=0; i<sectorCount; i++) {

        for (unsigned int x=0; x<TTL; x++) {
            *p = inw(0x1F0);

            //if ( *p > 0 ) printf("[%d, %c, %s]",*p, *p, *p);

            out[x] = *p;

            p++;
        }


        printf( "------------------------------------------- \n" );
        printf( "%s \n", out );

        inb(0x1F7);
        inb(0x1F7);
        inb(0x1F7);
        inb(0x1F7);//400ns delay

        //printf("while [1]\n");
        while (1) {
            status = inb(0x1F7);
            if (status & 0x80) { //BSY set
                // * printf(" ******** (status & 0x80)\n");
                continue;

            } else {
                // * printf(" ******** (status & 0x80): break\n");
                break;
            }

            if (status & 0x1) { //ERR set
                // * printf(" ******** (status & 0x1)\n");
                return 0;

            }

            if (status & 0x20) { //DF set
                // * printf(" ******** (status & 0x20)\n");
                return 0;
            }
        }

    }

    printf("ata_read_lba28: END\n");
    return 1;
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
