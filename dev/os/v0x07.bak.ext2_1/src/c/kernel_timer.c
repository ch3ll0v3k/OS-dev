#include "h/kernel.h"

// ====================================================================
// This will keep track of how many ticks that the system has been running for
// http://www.osdever.net/bkerndev/Docs/pit.htm
// http://ethv.net/workshops/osdev/notes/notes-4

// ====================================================================
void RTC_handler( REG *reg ) {

    printf("RTC \n" );

}
// ====================================================================
// uint8_t rtc_get_year() { outb(0x70, 0x09); return inb(0x71); }
// uint8_t rtc_get_month() { outb(0x70, 0x08); return inb(0x71); }
// uint8_t rtc_get_day() { outb(0x70, 0x07); return inb(0x71); }
// uint8_t rtc_get_weekday() { outb(0x70, 0x06); return inb(0x71); }
// uint8_t rtc_get_hour() { outb(0x70, 0x04); return inb(0x71); }
// uint8_t rtc_get_minute() { outb(0x70, 0x02); return inb(0x71); }
// uint8_t rtc_get_second() { outb(0x70, 0x00); return inb(0x71); }

/*void rtc_print_time() {

    uint32_t t_y = rtc_get_year();
    uint32_t t_m = rtc_get_month();
    uint32_t t_d = rtc_get_day();
    uint32_t t_h = rtc_get_hour();
    uint32_t t_i = rtc_get_minute();
    uint32_t t_s = rtc_get_second();
    printf(" %d.%d.%d %d:%d:%d \n", t_y,  t_m,  t_d,  t_h,  t_i,  t_s  );

}*/

/* while (1) {
    while ( !(TASK[0]) ) { __asm__ __volatile__("nop"); }
    printf("TASK: 0\n");
} */


#define TTL_CURRENT_PROCESSES 3
uint32_t CURRENT_PROCESS_ID = 0;

uint32_t PROCESSES_DATA[ TTL_CURRENT_PROCESSES ];
uint32_t PROCESSES[ TTL_CURRENT_PROCESSES ];

// ====================================================================
char PROCESS_CHAR_BUFF[80];
uint8_t char_b_i = 0;
uint8_t char_b_ii = 0;

void PROCESS_0() {
    PROCESSES_DATA[ CURRENT_PROCESS_ID ] += 3;
    sprintf( PROCESS_CHAR_BUFF, "3: %d       ", PROCESSES_DATA[ CURRENT_PROCESS_ID ] );

    char_b_i = 0;
    char_b_ii = 68;//80-12;
    while ( char_b_i < 12 ) {
        k_term_status_bar_buff[ char_b_ii ] = ' ';
        k_term_status_bar_buff[ char_b_ii++ ] = PROCESS_CHAR_BUFF[ char_b_i++ ];

    }

}

void PROCESS_1() {
    PROCESSES_DATA[ CURRENT_PROCESS_ID ] += 2;
    sprintf( PROCESS_CHAR_BUFF, "2: %d       ", PROCESSES_DATA[ CURRENT_PROCESS_ID ] );

    char_b_i = 0;
    char_b_ii = 56;//80-12-12;
    while ( char_b_i < 12 ) {
        k_term_status_bar_buff[ char_b_ii ] = ' ';
        k_term_status_bar_buff[ char_b_ii++ ] = PROCESS_CHAR_BUFF[ char_b_i++ ];

    }

}

void PROCESS_2() {
    PROCESSES_DATA[ CURRENT_PROCESS_ID ] += 1;
    sprintf( PROCESS_CHAR_BUFF, "1: %d       ", PROCESSES_DATA[ CURRENT_PROCESS_ID ] );

    char_b_i = 0;
    char_b_ii = 44;//80-12-12-12;
    while ( char_b_i < 12 ) {
        k_term_status_bar_buff[ char_b_ii ] = ' ';
        k_term_status_bar_buff[ char_b_ii++ ] = PROCESS_CHAR_BUFF[ char_b_i++ ];

    }


}




void TEST() {

    while ( 1 ) {
        outb(0x70, 0x00);
        RTC[5] = inb(0x71);

        uint32_t t_s = RTC[5];

        while ( t_s == RTC[5] ) {
            //_asm_nop();
            _asm_hlt();
            outb(0x70, 0x00); t_s = inb(0x71);
        }

        k_term_status_bar_clear();

    }

}

char *M_DAY[] = {
    "Sat",
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
};

void not_registered_func () {


    // Status Register B, Bit 1 (value = 2): Enables 24 hour format if set
    // Status Register B, Bit 2 (value = 4): Enables Binary mode if set
    // outb(0x0A, (1 << 0 ) ); // outb(0x0B, (1 << 0 ) );
    // outb(0x0A, (1 << 1 ) ); // outb(0x0B, (1 << 1 ) );
    // outb(0x0A, (1 << 2 ) ); // outb(0x0B, (1 << 2 ) );
    // outb(0x0A, (1 << 3 ) ); // outb(0x0B, (1 << 3 ) );
    // outb(0x0A, (1 << 4 ) ); // outb(0x0B, (1 << 4 ) );
    // outb(0x0A, (1 << 5 ) ); // outb(0x0B, (1 << 5 ) );
    // outb(0x0A, (1 << 6 ) ); // outb(0x0B, (1 << 6 ) );
    // outb(0x0A, (1 << 7 ) ); // outb(0x0B, (1 << 7 ) );

    while ( 1 ) {

        outb(0x70, 0x09); RTC[0] = inb(0x71); // 0x09      Year
        outb(0x70, 0x08); RTC[1] = inb(0x71); // 0x08      Month
        outb(0x70, 0x07); RTC[2] = inb(0x71); // 0x07      Day of Month
        outb(0x70, 0x04); RTC[3] = inb(0x71); // 0x04      Hours
        outb(0x70, 0x02); RTC[4] = inb(0x71); // 0x02      Minutes
        outb(0x70, 0x00); RTC[5] = inb(0x71); // 0x00      Seconds
        outb(0x70, 0x06); RTC[6] = inb(0x71); // 0x06      Weekday
        outb(0x70, 0x32); RTC[7] = inb(0x71); // 0x32      Century (maybe)

        uint32_t t_s = RTC[5];
        while ( t_s == RTC[5] ) { _asm_nop(); outb(0x70, 0x00); t_s = inb(0x71); }
        printf( " %s 20%x-%x-%x, %d:%x:%x \n", M_DAY[ RTC[2]-1 ], RTC[0], RTC[1], RTC[2], RTC[3]+2, RTC[4], RTC[5] );
        //k_term_status_bar_buff
        //k_term_status_bar_draw();

    }

}

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

    if (kernel_ticks == 20) {

        kernel_ticks = 0;
        tt++;

        CURRENT_PROCESS_ID++;
        if ( CURRENT_PROCESS_ID >= TTL_CURRENT_PROCESSES ) CURRENT_PROCESS_ID = 0;

        __asm__ __volatile__ ("call *%0 " : : "r" ( (uint32_t *) PROCESSES[ CURRENT_PROCESS_ID ]));
        k_term_status_bar_draw();

        char *BUFF = (char *)0xb8000 + TERM_TICKS_AREA;
        itoa( timer_handler_buff, tt, 10 );


        uint32_t len = strlen( timer_handler_buff );
        uint32_t i = 0;

        uint32_t BUFF_I = 0;

        BUFF[ BUFF_I++ ] = '['; BUFF[ BUFF_I++ ] = 0x0c;
        BUFF[ BUFF_I++ ] = ' '; BUFF[ BUFF_I++ ] = 0x0c;

        while ( i < len ) {
            BUFF[ BUFF_I++ ] = timer_handler_buff[i++]; BUFF[ BUFF_I++ ] = 0x0c;
        }

        BUFF[ BUFF_I++ ] = ' '; BUFF[ BUFF_I++ ] = 0x0c;
        BUFF[ BUFF_I++ ] = ']'; BUFF[ BUFF_I++ ] = 0x0c;

    }

    if ( reg->int_no > 10000 && reg->err_code > 10000 ) return;
}

// ====================================================================
void timer_install() {

    k_term_status_bar_clear();

    PROCESSES_DATA[0] = 0;
    PROCESSES_DATA[1] = 0;
    PROCESSES_DATA[2] = 0;

    PROCESSES[0] = (uint32_t *) PROCESS_0;
    PROCESSES[1] = (uint32_t *) PROCESS_1;
    PROCESSES[2] = (uint32_t *) PROCESS_2;



    tt = kernel_ticks = 0;
    //timer_phase(1193180/100);
    irq_install_handler(0, timer_handler); // Installs 'timer_handler' to IRQ0
    irq_install_handler(8, RTC_handler); // IRQ 8  => Real-time clock

}


// ====================================================================
void timer_wait( uint32_t ticks ) {

    ulong_t eticks;
    eticks = kernel_ticks + ticks;

    while (kernel_ticks < eticks);
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
