#include "h/kernel.h"

// ====================================================================
// http://www.osdever.net/bkerndev/Docs/pit.htm
// ====================================================================
// This will keep track of how many ticks that the system has been running for
uint64_t kernel_ticks = 0;

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

void timer_handler( REG *reg) {

    if ( reg->int_no > 10000 && reg->err_code > 10000 ) return;



    // Increment our 'tick count'
    kernel_ticks++;

    // Every 18 clocks (approximately 1 second), we will display a message on the screen
    // if (kernel_ticks % 18 == 0) {
    //
    if (kernel_ticks % 18 == 0) {

        char *BUFF = (char *)0xb8000 + TERM_TICKS_AREA;

        itoa( timer_handler_buff, kernel_ticks );

        uint32_t len = strlen( timer_handler_buff );
        uint32_t i = 0;

        uint32_t BUFF_I = 0;

        BUFF[ BUFF_I++ ] = ' ';
        BUFF[ BUFF_I++ ] = 0x0c;

        while ( i < len ) {

            BUFF[ BUFF_I++ ] = timer_handler_buff[i++];
            BUFF[ BUFF_I++ ] = 0x0c;

        }

        BUFF[ BUFF_I++ ] = ' ';
        BUFF[ BUFF_I++ ] = 0x0c;

    }
}

// ====================================================================
// Sets up the system clock by installing the timer handler into IRQ0
void timer_install() {

    //timer_phase(1193180/100);

    // Installs 'timer_handler' to IRQ0
    irq_install_handler(0, timer_handler);
}

// ====================================================================
// This will continuously loop until the given time has been reached
void timer_wait( uint32_t ticks ) {

    ulong_t eticks;
    eticks = kernel_ticks + ticks;

    while (kernel_ticks < eticks);
}

// ====================================================================