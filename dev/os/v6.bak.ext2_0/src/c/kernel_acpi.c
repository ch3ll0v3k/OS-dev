#include "h/kernel.h"

// ====================================================================
#define PRIMARY_BASE 0x1F0

#define ATA_READ 0x24
#define ATA_WRITE 0x34

// ====================================================================

// " Current disk controller chips almost always support two ATA buses per chip. ";
// " There is a standardized set of IO ports to control the disks on the buses. ";
// " The first two buses are called the Primary and Secondary ATA bus, and are almost always ";
// " controlled by IO ports 0x1F0 through 0x1F7, and 0x170 through 0x177, respectively (unless you change it).";
// " The associated Device Control Registers/Alternate Status ports are IO ports 0x3F6, and 0x376, respectively. ";
// " The standard IRQ for the Primary bus is IRQ14, and IRQ15 for the Secondary bus.";
// " If the next two buses exist, they are normally controlled by IO ports 0x1E8 through 0x1EF, ";
// " and 0x168 through 0x16F, respectively. The associated Device Control Registers/Alternate Status ";
// " ports are IO ports 0x3E6, and 0x366. ";

// ====================================================================
// irq14(); // IRQ 14 => Primary IDE channel   This interrupt is reserved for use by the primary IDE controller. On systems that do not use IDE devices, the IRQ can be used for another purpose.
// irq15(); // IRQ 15 => Secondary IDE channel This interrupt is reserved for use by the secondary IDE controller.

//irq_install_handler(14, primary_IDE_handler );
//irq_install_handler(15, secondary_IDE_handler );
// ====================================================================
// drive could be 0x1F7 or 0x170 and args 0xA0 or 0xB0
/*
uint8_t poll_drive(uint16_t drive, uint8_t arg) {

    outb(drive, arg);

    _sleep(10); // ATA [400ns delays]
    return inb(drive) & (1 << 6);

}

*/

// ====================================================================
/*
void read_hdd_lba28( uint16_t *buffer, uint8_t addr, uint8_t sector_count, uint16_t drive, uint8_t is_slave ) {

    outb(drive + 1, 0x00);
    outb(drive + 2, sector_count);
    outb(drive + 3, (uint8_t) ((addr) & 0xFF));
    outb(drive + 4, (uint8_t)((addr >> 8) & 0xFF));
    outb(drive + 5, (uint8_t)((addr >> 16) & 0xFF));
    outb(drive + 6,  0xE0 | (is_slave << 4) | ((addr >> 24) & 0x0F));
    outb(drive + 7, ATA_READ);

    while (!(inb(drive + 7) & 0x08));

    for (uint32_t i = 0; i < 256; i++) {

        uint16_t tmpword = inw(drive);
        buffer[i * 2] = (uint8_t)tmpword;
        buffer[i * 2 + 1] = (uint8_t)(tmpword >> 8);
    }

}
*/
// ====================================================================
/*
void read_ata() {

    //char addrbuf[4];
    //char secbuf[4];
    uint32_t addr = 0;
    uint32_t sec_ct = 8;

    //printf("Start addr: ");
    //getstr(addrbuf);
    //printf("Sector count: ");
    //getstr(secbuf);

    //addr = atoi(addrbuf);
    //sec_ct = atoi(secbuf);

    uint16_t buff[ 512 * sec_ct ];

    read_hdd_lba28(buff, addr, sec_ct, (uint16_t)PRIMARY_BASE, 0); // primbase is 0x1F0

    for (uint32_t i = 0; i < 256 * sec_ct; i++) {
        //printf("0x%s ", itoa(buff[i], 0, 16));


        //itoa(buff[i], i, 16);

        //printf("0x%s ", itoa(buff[i], 0, 16));
    }

    printf("\n");

}
*/
// ====================================================================
/*
void read_command() {
    char addrbuf[4];
    char secbuf[4];
    uint32_t addr = 0;
    uint32_t sec_ct = 0;

    printf("Start addr: ");
    getstr(addrbuf);
    printf("Sector count: ");
    getstr(secbuf);

    addr = atoi(addrbuf);
    sec_ct = atoi(secbuf);

    unsigned short buf[256 * sec_ct];

    if (!io_hdd_lba28(ATA_CMD_READ_PIO, buf, addr, sec_ct, PRIMARY_BASE, 0)) {
        printf("Failed to read drive..\n");
        return;
    }

    for (int i = 0; i < 256 * sec_ct; i++) {
        printf("%s ", itoa((unsigned short) (buf[i]), 0, 16));
    }
    printf("\n");
}
*/

// ====================================================================
