#include "h/kernel.h"

// ====================================================================
void kernel_init() {

    //-----------------------------------------------------------------
    KERNEL_BOOT_INFO = 1;
    k_term_clear();

    //__OS__();

    //-----------------------------------------------------------------
    if (KERNEL_BOOT_INFO) kernel_log_info("kernel: init");

    if (KERNEL_BOOT_INFO) kernel_log_info("k_term");
    k_term_init();

    if (KERNEL_BOOT_INFO) kernel_log_info("gdt-registers");
    init_gdt_register();

    if (KERNEL_BOOT_INFO) kernel_log_info("idt-registers");
    init_idt_register();


    timer_install();
    kb_install();

    if (KERNEL_BOOT_INFO) kernel_log_info("done\n");
    usleep(250);

    //-----------------------------------------------------------------
    k_term_clear();
    //-----------------------------------------------------------------
}

// ====================================================================
void test() {
    k_term_printnl( " *** TEST ***\n");
}

// ====================================================================
//void kernel_main( char* MK_struct, unsigned int MAGIC ) {
void kernel_main() {

    //-----------------------------------------------------------------
    kernel_init();

    usleep( 500 );
    reset_promt();

    while (1) {
        _asm_hlt();
    }

    printf( "kernel_main: FATAL-ERROR\n");

    //-----------------------------------------------------------------

}

// ====================================================================
void kernel_mknot() {
    kernel_log_ok(" *** [kernel_mknot] ***\n");
    usleep(500);
}

void kernel_mknot_2() {
    kernel_log_ok(" *** [kernel_mknot_2] ***\n");
    usleep(500);

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