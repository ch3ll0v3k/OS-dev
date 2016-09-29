#include "include/kernel.h"
// ====================================================================

// ====================================================================
void kernel_main( char* MK_struct, unsigned int MAGIC ) {
    //void kernel_main() {

    k_term_clear();

    kernel_log_info( "This is kernel info " );
    kernel_log_error( "This is kernel error " );
    kernel_log_std( "This is kernel std " );
    kernel_log_ok( "This is kernel ok " );

    k_term_printnl("------------------------------");
    k_term_printnl(" line 1");
    k_term_printnl("------------------------------");

    k_term_print_intnl( 777 );

    k_term_print_int( 123 );

    while (1);

}

// ====================================================================