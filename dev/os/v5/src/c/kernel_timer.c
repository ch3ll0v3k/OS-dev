#include "h/kernel.h"

// ====================================================================
// This will keep track of how many ticks that the system has been running for
// http://www.osdever.net/bkerndev/Docs/pit.htm
// http://ethv.net/workshops/osdev/notes/notes-4

// ====================================================================
void timer_phase(int hz) {

    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

// ====================================================================
// Handles the timer. In this case, it's very simple: We
// increment the 'kernel_ticks' variable every time the
// timer fires. By default, the timer fires 18.222 times
// per second. Why 18.222Hz? Some engineer at IBM must've
// been smoking something funky

char timer_handler_buff[256];

uint32_t tt;

void timer_handler( REG *reg) {

    kernel_ticks++;

    // Every 18 clocks (approximately 1 second), we will display a message on the screen
    // if (kernel_ticks % 18 == 0) {
    //
    if (kernel_ticks == 20) {

        kernel_ticks = 0;
        tt++;

        char *BUFF = (char *)0xb8000 + TERM_TICKS_AREA;
        itoa( timer_handler_buff, tt, 10 );


        uint32_t len = strlen( timer_handler_buff );
        uint32_t i = 0;

        uint32_t BUFF_I = 0;

        BUFF[ BUFF_I++ ] = '[';
        BUFF[ BUFF_I++ ] = 0x0c;

        BUFF[ BUFF_I++ ] = ' ';
        BUFF[ BUFF_I++ ] = 0x0c;

        while ( i < len ) {

            BUFF[ BUFF_I++ ] = timer_handler_buff[i++];
            BUFF[ BUFF_I++ ] = 0x0c;

        }

        BUFF[ BUFF_I++ ] = ' ';
        BUFF[ BUFF_I++ ] = 0x0c;
        BUFF[ BUFF_I++ ] = ']';
        BUFF[ BUFF_I++ ] = 0x0c;

    }

    if ( reg->int_no > 10000 && reg->err_code > 10000 ) return;
}

// ====================================================================
// * void irq_hand_0( REG *reg ) { k_term_printnl(" irq [0]\n" ); }
// * void irq_hand_1( REG *reg ) { k_term_printnl(" irq [1]\n" ); }
// * void irq_hand_2( REG *reg ) { k_term_printnl(" irq [2]\n" ); }
// * void irq_hand_3( REG *reg ) { k_term_printnl(" irq [3]\n" ); }
// * void irq_hand_4( REG *reg ) { k_term_printnl(" irq [4]\n" ); }
// * void irq_hand_5( REG *reg ) { k_term_printnl(" irq [5]\n" ); }
// * void irq_hand_6( REG *reg ) { k_term_printnl(" irq [6]\n" ); }
// * void irq_hand_7( REG *reg ) { k_term_printnl(" irq [7]\n" ); }
// * void irq_hand_8( REG *reg ) { k_term_printnl(" irq [8]\n" ); }
// * void irq_hand_9( REG *reg ) { k_term_printnl(" irq [9]\n" ); }
// * void irq_hand_10( REG *reg ) { k_term_printnl(" irq [10]\n" ); }
// * void irq_hand_11( REG *reg ) { k_term_printnl(" irq [11]\n" ); }
// * void irq_hand_12( REG *reg ) { k_term_printnl(" irq [12]\n" ); }
// * void irq_hand_13( REG *reg ) { k_term_printnl(" irq [13]\n" ); }
// * void irq_hand_14( REG *reg ) { k_term_printnl(" irq [14]\n" ); }
// * void irq_hand_15( REG *reg ) { k_term_printnl(" irq [15]\n" ); }
// * void irq_hand_16( REG *reg ) { k_term_printnl(" irq [16]\n" ); }
// ====================================================================
// Sets up the system clock by installing the timer handler into IRQ0
void timer_install() {

    tt = 0;
    kernel_ticks = 0;
    //timer_phase(1193180/100);

    // Installs 'timer_handler' to IRQ0
    irq_install_handler(0, timer_handler);

    // * irq_install_handler(0, irq_hand_0);
    // * irq_install_handler(1, irq_hand_1);
    // * irq_install_handler(2, irq_hand_2);
    // * irq_install_handler(3, irq_hand_3);
    // * irq_install_handler(4, irq_hand_4);
    // * irq_install_handler(5, irq_hand_5);
    // * irq_install_handler(6, irq_hand_6);
    // * irq_install_handler(7, irq_hand_7);
    // * irq_install_handler(8, irq_hand_8);
    // * irq_install_handler(9, irq_hand_9);
    // * irq_install_handler(10, irq_hand_10);
    // * irq_install_handler(11, irq_hand_11);
    // * irq_install_handler(12, irq_hand_12);
    // * irq_install_handler(13, irq_hand_13);
    // * irq_install_handler(14, irq_hand_14);
    // * irq_install_handler(15, irq_hand_15);

}


// ====================================================================
// This will continuously loop until the given time has been reached
void timer_wait( uint32_t ticks ) {

    ulong_t eticks;
    eticks = kernel_ticks + ticks;

    while (kernel_ticks < eticks);
}

// ====================================================================