#include "h/kernel.h"

// ====================================================================
void kernel_init( uint32_t magic, uint32_t MK_struct ) {

    //-----------------------------------------------------------------
    KERNEL_BOOT_INFO = 0;
    KERNEL_MEM_INFO = 0;
    KERNEL_INFO_DELAY = 10;

    //-----------------------------------------------------------------
    multiboot_reg_global = (multiboot_info_t *) MK_struct;
    multiboot_magic_global = magic;

    //-----------------------------------------------------------------
    if (KERNEL_BOOT_INFO) kernel_log_info("kernel: init");
    if (KERNEL_BOOT_INFO) kernel_log_info("k_terminal");
    k_term_init();
    k_term_clear();

    ///////////////////////////////////////////////////////////////////////////////////////
    /*
    multiboot_info_t mbb = *((multiboot_info_t *) MK_struct);

    printf( "flags              [%d] [0x%x]\n:", mbb.flags, mbb.flags ); // Multiboot info version number
    printf( "mem_lower          [%d] [0x%x]\n:", mbb.mem_lower, mbb.mem_lower ); // Available memory from BIOS
    printf( "mem_upper          [%d] [0x%x]\n:", mbb.mem_upper, mbb.mem_upper );
    printf( "boot_device        [%d] [0x%x]\n:", mbb.boot_device, mbb.boot_device ); // "root" partition
    printf( "cmdline            [%s] \n:", mbb.cmdline ); // Kernel command line
    printf( "mods_count         [%d] [0x%x]\n:", mbb.mods_count, mbb.mods_count ); // Boot-Module list
    printf( "mods_addr          [%d] [0x%x]\n:", mbb.mods_addr, mbb.mods_addr );
    printf( "mbb.u.aout_sym     [%d] [0x%x]\n:", mbb.u.aout_sym, mbb.u.aout_sym ); // UNION
    printf( "mbb.u.elf_sec      [%d] [0x%x]\n:", mbb.u.elf_sec, mbb.u.elf_sec ) ;  // UNION
    printf( "mmap_length        [%d] [0x%x]\n:", mbb.mmap_length, mbb.mmap_length ); // Memory Mapping buffer
    printf( "------------------------------\n:");
    printf( "mmap_addr          [%d] [0x%x]\n:", mbb.mmap_addr, mbb.mmap_addr );
    printf( "drives_length      [%d] [0x%x]\n:", mbb.drives_length, mbb.drives_length ); // Drive Info buffer
    printf( "drives_addr        [%d] [0x%x]\n:", mbb.drives_addr, mbb.drives_addr );
    printf( "config_table       [%d] [0x%x]\n:", mbb.config_table, mbb.config_table ); // ROM configuration table
    printf( "boot_loader_name   [%s]\n:", mbb.boot_loader_name, mbb.boot_loader_name ); // Boot Loader Name
    printf( "apm_table          [%d] [0x%x]\n:", mbb.apm_table, mbb.apm_table ); // APM table
    printf( "vbe_control_info   [%d] [0x%x]\n:", mbb.vbe_control_info, mbb.vbe_control_info ); // Video
    printf( "vbe_mode_info      [%d] [0x%x]\n:", mbb.vbe_mode_info, mbb.vbe_mode_info );
    printf( "vbe_mode           [%d] [0x%x]\n:", mbb.vbe_mode, mbb.vbe_mode );
    printf( "vbe_interface_seg  [%d] [0x%x]\n:", mbb.vbe_interface_seg, mbb.vbe_interface_seg );
    printf( "vbe_interface_off  [%d] [0x%x]\n:", mbb.vbe_interface_off, mbb.vbe_interface_off );
    printf( "vbe_interface_len  [%d] [0x%x]\n:", mbb.vbe_interface_len, mbb.vbe_interface_len );
    */
    ///////////////////////////////////////////////////////////////////////////////////////
    /*
    boot_info bf = *((boot_info *) MK_struct);
    printf( "flags:                 [%d] [0x%x]\n", bf.flags, bf.flags );
    printf( "mem_lower:             [%d] [0x%x]\n", bf.mem_lower, bf.mem_lower );
    printf( "mem_upper:             [%d] [0x%x]\n", bf.mem_upper, bf.mem_upper );
    printf( "boot_device:           [%d] [0x%x]\n", bf.boot_device, bf.boot_device );
    printf( "cmdline:               [%s] [0x%x]\n", bf.cmdline, bf.cmdline );
    printf( "mods_count:            [%d] [0x%x]\n", bf.mods_count, bf.mods_count );
    printf( "mods_addr:             [%d] [0x%x]\n", bf.mods_addr, bf.mods_addr );
    printf( "num:                   [%d] [0x%x]\n", bf.num, bf.num );
    printf( "size:                  [%d] [0x%x]\n", bf.size, bf.size );
    printf( "addr:                  [%d] [0x%x]\n", bf.addr, bf.addr );
    printf( "shndx:                 [%d] [0x%x]\n", bf.shndx, bf.shndx );
    printf( "mmap_length:           [%d] [0x%x]\n", bf.mmap_length, bf.mmap_length );
    printf( "mmap_addr:             [%d] [0x%x]\n", bf.mmap_addr, bf.mmap_addr );
    printf( "drives_length:         [%d] [0x%x]\n", bf.drives_length, bf.drives_length );
    printf( "drives_addr:           [%d] [0x%x]\n", bf.drives_addr, bf.drives_addr );
    printf( "config_table:          [%d] [0x%x]\n", bf.config_table, bf.config_table );
    printf( "boot_loader_name:      [%s] [0x%x]\n", bf.boot_loader_name, bf.boot_loader_name );
    printf( "apm_table:             [%d] [0x%x]\n", bf.apm_table, bf.apm_table );
    printf( "vbe_control_info:      [%d] [0x%x]\n", bf.vbe_control_info, bf.vbe_control_info );
    printf( "vbe_mode_info:         [%d] [0x%x]\n", bf.vbe_mode_info, bf.vbe_mode_info );
    printf( "vbe_mode:              [%d] [0x%x]\n", bf.vbe_mode, bf.vbe_mode );
    printf( "vbe_interface_seg:     [%d] [0x%x]\n", bf.vbe_interface_seg, bf.vbe_interface_seg );
    printf( "vbe_interface_off:     [%d] [0x%x]\n", bf.vbe_interface_off, bf.vbe_interface_off );
    printf( "vbe_interface_len:     [%d] [0x%x]\n", bf.vbe_interface_len, bf.vbe_interface_len );
    printf( "framebuffer_addr:      [%d] [0x%x]\n", bf.framebuffer_addr, bf.framebuffer_addr );
    printf( "framebuffer_pitch:     [%d] [0x%x]\n", bf.framebuffer_pitch, bf.framebuffer_pitch );
    printf( "framebuffer_width:     [%d] [0x%x]\n", bf.framebuffer_width, bf.framebuffer_width );
    printf( "framebuffer_height:    [%d] [0x%x]\n", bf.framebuffer_height, bf.framebuffer_height );
    printf( "framebuffer_bpp:       [%d] [0x%x]\n", bf.framebuffer_bpp, bf.framebuffer_bpp );
    printf( "framebuffer_type:      [%d] [0x%x]\n", bf.framebuffer_type, bf.framebuffer_type );
    return;
    */

    ///////////////////////////////////////////////////////////////////////////////////////
    if (KERNEL_MEM_INFO) kernel_mem_info( );

    if (KERNEL_BOOT_INFO) kernel_log_info("GDT registers");
    init_gdt_register();

    if (KERNEL_BOOT_INFO) kernel_log_info("IDT registers");
    init_idt_register();

    if (KERNEL_BOOT_INFO) kernel_log_info("SYS TIMER");
    timer_install();

    if (KERNEL_BOOT_INFO) kernel_log_info("KB and IRQs");
    kb_install();

    if (KERNEL_BOOT_INFO) kernel_log_info("install ATA handlers");
    INSTALL_ATA_HANDLERS();

    if (KERNEL_BOOT_INFO) kernel_log_info("init ATA-UTILS");
    if (KERNEL_BOOT_INFO) kernel_log_info(" detect ATA-DRIVES");
    init_ata_utils();

    if (KERNEL_BOOT_INFO) kernel_log_info(" ... \n\n");

    //-----------------------------------------------------------------
    k_term_clear();
    if (KERNEL_BOOT_INFO) { k_term_clear(); __OS__(); }
    k_term_clear();

    k_term_clean_up_env();
    //-----------------------------------------------------------------
}

// ====================================================================
// The magic field should contain this.
// #define MULTIBOOT_HEADER_MAGIC                  0x1BADB002

// This should be in %eax.
// #define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

// ====================================================================
void kernel_main( uint32_t MK_struct, uint32_t magic ) {

    //-----------------------------------------------------------------
    kernel_init( MK_struct, magic );

    // get_hd_info( 0 );
    // TMX_FS_FORMAT( 0 );

    // get_hd_info( 1 );
    // get_hd_info( 2 );
    // get_hd_info( 3 );

    // not_registered_func ();
    //kernel_init_ext2_fs_support();

    //TMX_FS_MK_DIR( "--", "--" );

    //-----------------------------------------------------------------
    /*
    uint32_t _ATA_LBA = 0;

    printf("========================================================\n" );
    ext2_directory();
    usleep(400);

    printf("[%d]-------------------------------------\n", _ATA_LBA );
    set_LBA_SEC_CNT_WRITE( _ATA_LBA );
    ext2_directory(); usleep(400);
    */

    //READ_CHAR_T_512b();

    /*
    while ( _ATA_LBA++ < 24 ) {

        printf("[%d]-------------------------------------\n", _ATA_LBA );
        set_LBA_SEC_CNT_WRITE( _ATA_LBA );
        ext2_directory(); usleep(400);

    }*/

    //-----------------------------------------------------------------
    goto __END__;

__END__:

    reset_promt();

    while (1) {
        _asm_hlt();
    }

    //-----------------------------------------------------------------

}

// ====================================================================
void kernel_mknot() {
    kernel_log_ok(" *** [kernel_mknot] ***\n");
    //usleep(500);
}

void kernel_mknot_2() {
    kernel_log_ok(" *** [kernel_mknot_2] ***\n");
    //usleep(500);

}

// ====================================================================
void IRQ_HANDLER( REG *reg ) {

    //if ( reg->int_no > 10000 && reg->err_code > 10000 ) return;

    //printf("IRQ_HANDLER: [REG *]\n");
    //usleep(1000);

    // This is a blank function pointer
    void (*handler)( REG *reg);

    // Find out if we have a custom handler to run for this
    // IRQ, and then finally, run it
    handler = irq_routines[ reg->int_no - 32 ];
    if (handler) {
        handler(reg);
    }

    // If the IDT entry that was invoked was greater than 40
    // (meaning IRQ8 - 15), then we need to send an EOI to
    // the slave controller
    if ( reg->int_no >= 40 ) {
        outb(0xA0, 0x20);
    }

    // In either case, we need to send an EOI to the master
    // interrupt controller too
    outb(0x20, 0x20);


}


// ====================================================================

/*
    sleep TEST

    int i = 0;
    while ( i < 500 ) {
        printf("line num: %d\n", i);
        //usleep( 1000 );
        sleep( 1 );
        i++;
    }
*/

/*

    // strcmp(*) TEST
    char *str_A = "abcd";
    char *str_B = "abcc";

    if ( strcmp( str_A, str_B ) == 0 ) {
        k_term_printnl("strings are == ");

    } else {
        k_term_printnl("strings are != ");

    }
*/


/*
    kernel_log_info( "This is kernel info " );
    kernel_log_error( "This is kernel error " );
    kernel_log_std( "This is kernel std " );
    kernel_log_ok( "This is kernel ok " );

    k_term_printnl("------------------------------");
    k_term_printnl(" line 1");
    k_term_printnl("------------------------------");
    k_term_print_intnl( 777 );
    k_term_print_int( 123 );
*/


/*
    // printf(*); TEST

    printf("[%c] - [%c] - [%c]\n", 'Z', 'X', 'Y' );
    printf("[%d] - [%d] - [%d]\n", 0, 1, 2 );
    printf("[%s] - [%d] - [%c]\n", "str", 123, 'A' );
    printf( "[this is string]" );

    //char city[] = "Los-Angeles";
    //char country[] = "usa";
    //char street[] = "BroadStreet";
    //uint32 zip = 123456;
    //uint32 house_num = 99;

    //char str[512];
    //sprintf( str, "country [%s] city [%s] zip [%d] \n street [%s], nr [%d]\n", country, city, zip, street, house_num );
    //k_term_printnl( str );

*/