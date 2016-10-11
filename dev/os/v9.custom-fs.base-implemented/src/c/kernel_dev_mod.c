#include "h/kernel.h"

// ====================================================================

void kb_handler( REG *reg) {

    if ( reg->int_no > 10000 && reg->err_code > 10000 ) return;
    //k_term_process_input_reading = 1;
    //k_term_process_input( r );

    /*if ( inportb(0x64) & 0x1 ) {
        //_inportb_input = inportb(0x60);
        //printf("%d ", _inportb_input);

    }*/

}


void kb_install() {

    //irq_install_handler( 1, kb_handler );
    irq_install_handler( 1, k_term_process_input );

}















// ====================================================================
