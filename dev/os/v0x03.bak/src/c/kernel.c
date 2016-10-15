#include "h/kernel.h"

int test(int i);

// ====================================================================
void kernel_init() {

    KERNEL_BOOT_INFO = 1;

    k_term_clear();
    if (KERNEL_BOOT_INFO) kernel_log_info("kernel: init");

    if (KERNEL_BOOT_INFO) kernel_log_info("k_term");
    k_term_init();
    if (KERNEL_BOOT_INFO) kernel_log_info("gdt-registers");
    init_gdt_register();

    if (KERNEL_BOOT_INFO) kernel_log_info("idt-registers");
    init_idt_register();

    if (KERNEL_BOOT_INFO) kernel_log_info("pic:");
    init_pic();

    if (KERNEL_BOOT_INFO) kernel_log_info(" :: reinit interrupts");
    __asm__ __volatile__( "sti" : : );

    if (KERNEL_BOOT_INFO) kernel_log_info("done\n");




}


// ====================================================================
//void kernel_main( char* MK_struct, unsigned int MAGIC ) {
void kernel_main() {

    kernel_init();

    char input_out[1024];


    uint32 main_loop_c = 0;

    while (1) {
        k_term_get_input( input_out );
        //printf("start test()\n");
        //test( 20 );
        //_usleep(200);
        //int x =3/0;

        //printf( "main_loop_c: %d\n", ++main_loop_c );

    }

    printf( "kernel_main: FATAL-ERROR\n");

    while (1);
    //k_term_clear_line(0);

}

// ====================================================================

int test(int i) {

    int b;
    int X = 10;

    for ( ; X >= 0; X-- ) {
        printf("test(%d): \n", X);

        if ( X == 0 ) {
            printf("ZERO\n");
            _usleep(100);
            //_asm_hlt();

            printf("b = i / 0: [0]\n");
            b = i / X;
            printf("b = i / 0: [1]\n");
            //_asm_hlt();

        }

        b = i / X;



    }

    return b;

}

// ====================================================================

/*
    sleep TEST

    int i = 0;
    while ( i < 500 ) {
        printf("line num: %d\n", i);
        //_usleep( 1000 );
        _sleep( 1 );
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