#include "h/kernel.h"

// ====================================================================
void kernel_init( ulong_t MK_struct, ulong_t magic ) {

    //-----------------------------------------------------------------
    KERNEL_BOOT_INFO = 0;
    KERNEL_MEM_INFO = 0;
    KERNEL_INFO_DELAY = 1;

    //-----------------------------------------------------------------
    k_term_clear();

    multiboot_reg_global = (multiboot_info_t *) MK_struct;
    multiboot_magic_global = magic;

    if (KERNEL_MEM_INFO) kernel_mem_info( );

    //-----------------------------------------------------------------
    if (KERNEL_BOOT_INFO) kernel_log_info("kernel: init");

    if (KERNEL_BOOT_INFO) kernel_log_info("k_terminal");
    k_term_init();

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

    if (KERNEL_BOOT_INFO) kernel_log_info(" ... \n\n");

    //-----------------------------------------------------------------
    k_term_clear();
    //if (KERNEL_BOOT_INFO) { k_term_clear(); __OS__(); }
    k_term_clear();
    //reset_promt();
    //-----------------------------------------------------------------
}

// ====================================================================
// The magic field should contain this.
// #define MULTIBOOT_HEADER_MAGIC                  0x1BADB002

// This should be in %eax.
// #define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

// ====================================================================
char c2h(char c) {
    return "0123456789abcdef"[0x0F & (unsigned char)c];
}

// ====================================================================
void kernel_main( ulong_t MK_struct, ulong_t magic ) {

    //-----------------------------------------------------------------
    kernel_init( MK_struct, magic );
    // not_registered_func ();

    kernel_init_ext2_fs_support();



    //-----------------------------------------------------------------
    // INT CHAR 47 -> 57
    // uint32_t i = 45;
    // while ( i <= 57 ) printf("%c, ", i++ );

    /*
    //uint32_t _int = 703710; // abcde
    uint32_t _int = 123456789; // 75bcd15
    //uint32_t _int = 15709004; // efb34c

    char buff[32];
    itoa( buff, _int, 16 );
    strupper(buff);

    printf(" INT:[%d] == [0x%s] \n", _int, buff );
    //printf(" INT:[%d] == [0x%x] \n", _int, _int );
    if ( strcmp( "75BCD15", buff ) == 0 ) k_term_printnl("SAME");

    // HEX COUNTER
    uint32_t ii = 0;
    while ( ii < 65536 ) {
        printf("[%d] >> [0x%x]\n", ii, ii );
        ii++;
        if ( ii < 50) usleep(20);
    }

    */
    goto __END__;
    //-----------------------------------------------------------------







    //-----------------------------------------------------------------
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