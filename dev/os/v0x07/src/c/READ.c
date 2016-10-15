// ====================================================================
void set_LBA_SEC_CNT( uint32_t _ATA_LBA, uint32_t _ATA_SEC_CNT ) {

    // ------------------------------------------------------------
    _ATA_LBA     = 1;
    _ATA_SEC_CNT = 1;

    outb( 0x1F0 + 2, (uint8_t)( _ATA_SEC_CNT ) );
    outb( 0x1F0 + 3, (uint8_t)((_ATA_LBA) & 0xFF) );       // low byte
    outb( 0x1F0 + 4, (uint8_t)((_ATA_LBA >> 8) & 0xFF) );  // mid byte
    outb( 0x1F0 + 5, (uint8_t)((_ATA_LBA >> 16) & 0xFF) ); // high byte
    outb( 0x1F0 + 6, 0xE0 | (0xE0 << 4) | ((_ATA_LBA >> 24) & 0x0F));
    outb( 0x1F0 + 7, 0xC4);
    // 0x20  Read Sectors (with retries)
    // 0x21  Read Sectors ( no  retries)
    // 0x29: READ MULTIPLE EXT
    // 0xC4: READ MULTIPLE SECTORS
    // -----------------------------------------------------------
    ATA_WAIT_400ns( 0x3F6 ); // ATA_GET_READY( ATA_0, ATA_0_CTRL );

    while ( (inb( 0x1F0 + 7) & 0x80) ) {
        printf("ATA_WAIT ... \n");
    }

    ATA_READ(); // просто читаю инфу

    // -----------------------------------------------------------

}

// ====================================================================
