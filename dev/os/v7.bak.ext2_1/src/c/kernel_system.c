#include "h/kernel_system.h"

// ====================================================================
//void asm_cmd(char * cmd) { __asm__ __volatile__( cmd ); }
// ====================================================================
/*
void apm_power_off( ) {

    uint8_t po_bios_call[] = {
        0xb8, 0x00, 0x10,   movw  $0x1000, ax
        0x8e, 0xd0,         movw  ax, ss
        0xbc, 0x00, 0xf0,   movw  $0xf000, sp
        0xb8, 0x07, 0x53,   movw  $0x5307, ax
        0xbb, 0x01, 0x00,   movw  $0x0001, bx
        0xb9, 0x03, 0x00,   movw  $0x0003, cx
        0xcd, 0x15          int   $0x15
    };

    // Some bioses don't like being called from CPU != 0
    if (apm_info.realmode_power_off) {
        set_cpus_allowed_ptr(current, cpumask_of(0));
        machine_real_restart(po_bios_call, sizeof(po_bios_call));

    } else {
        (void)set_system_power_state(APM_STATE_OFF);

    }

}
*/

// ====================================================================
/*
void idedriveinfo(ushort device) {
    inb(device + IDEERROR); //0x1F1
    idedrivewait(device, IDESTATUSREADY + IDESTATUSSEEKCOMPLETE); //0x1F0, 01010000b
    outportb(device + IDEDRIVEHEAD, IDEDRIVEHEADRESERVED + IDEDRIVEHEADPRIMARY); //0x1F6, 0xA0
    outportb(device + IDECOMMAND, IDECOMMANDIDENT); //0x1F7, 0xEC
    idedrivewait(device, IDESTATUSREADY + IDESTATUSSEEKCOMPLETE + IDESTATUSDRQ); //0x1F0, 01011000b

    kprintf("IDE Probe of Device at: 0x%h\n", device);
    kprintf("Flags: %b%b \n", inb(device), inb(device));

    uint32_t i = 0;

    for (i = 10; i>0; i--) {
        inb(device);
        inb(device);
    }

    char serial[21];
    serial[20] = 0;

    for (i=20; i>0; i--)
        serial[i-1] = inb(device);

    kprintf(serial);
}
*/
// ====================================================================
// [BYTE]
uint8_t inb ( uint16_t port ) {
    uint8_t r;
    __asm__ __volatile__ ( "inb %w1, %b0" : "=a" (r) : "d" (port) );
    return r;
}

void outb ( uint16_t port, uint8_t data ) {
    __asm__ __volatile__ ( "outb %b0, %w1" : : "a" (data), "d" (port) );
}


/*
// [WORD]
uint16_t inw( uint16_t port ) {
    uint16_t r;
    __asm__ __volatile__ ( "inw %w1, %w0" : "=a" (r) : "d" (port) );
    return r;

}
*/



uint16_t inw( uint16_t _port ) {
    uint16_t r;
    __asm__ __volatile__ ( "inw %1, %0" : "=a" (r) : "dN" (_port) );
    return r;

}









void outw( uint16_t port, uint16_t data ) {
    __asm__ __volatile__ ( "outw %w0, %w1" : : "a" (data), "d" (port) );

}

// [32bit] [STRING]
void outsw ( uint16_t port, const void *data, uint16_t size ) {
    __asm__ __volatile__ (
        "cld\n\t"
        "rep outsw\n\t"
        : "+S" (data), "+c" (size) : "d" (port)
    );
}

void insw ( uint16_t port, void *data, uint16_t size ) {
    __asm__ __volatile__ (
        "cld\n\t"
        "rep insw\n\t"
        : "+D" (data), "+c" (size) : "d" (port)
    );
}

// ====================================================================
/*
uint8_t inportb( uint16_t _port ) {
    uint8_t r;
    __asm__ __volatile__ ( "inb %1, %0" : "=a" (r) : "dN" (_port) );
    return r;

}

void outportb( uint16_t _port, uint8_t _data ) {
    __asm__ __volatile__ ( "outb %1, %0" : : "dN" (_port), "a" (_data) );

}


uint8_t inportw( uint16_t _port ) {
    uint16_t r;
    __asm__ __volatile__ ( "inw %1, %0" : "=a" (r) : "dN" (_port) );
    return r;

}

void outportw( uint16_t _port, uint16_t _data ) {
    __asm__ __volatile__ ( "outw %1, %0" : : "dN" (_port), "a" (_data) );

}

*/


/*
static inline void outb ( uint16_t port, uint8_t data ) {
    __asm__ __volatile__ ( "outb %b0, %w1" : : "a" (data), "d" (port) );
}

// Read from 8-bit port
// param port  Port number
// returns Read data
static inline uint8_t inb ( uint16_t port ) {
    uint8_t r;
    __asm__ __volatile__ ( "inb %w1, %b0" : "=a" (r) : "d" (port) );
    return r;
}
*/

// Write to 16-bit port
// param port  Port number
// param data  Data to be sent
/*
static inline void outw ( uint16_t port, uint16_t data ) {
    __asm__ __volatile__ ( "outw %w0, %w1" : : "a" (data), "d" (port) );
}*/

// Read from 16-bit port
// param port  Port number
// returns Read data
/*
static inline uint16_t inw ( uint16_t port ) {
    uint16_t r;

    __asm__ __volatile__ ( "inw %w1, %w0" : "=a" (r) : "d" (port) );
    return r;
}*/

// Write string to 16-bit port
// port: Port number
// data: String to be sent
// size: Size of Data
/*
static inline void outsw ( uint16_t port, const void *data, uint16_t size ) {
    __asm__ __volatile__ (
        "cld\n\t"
        "rep outsw\n\t"
        : "+S" (data), "+c" (size) : "d" (port)
    );
}*/

// Read string from 16-bit port
// port: Port number
// data: Address where to store read string
// size: Size of Data
/*
static inline void insw ( uint16_t port, void *data, uint16_t size ) {
    __asm__ __volatile__ (
        "cld\n\t"
        "rep insw\n\t"
        : "+D" (data), "+c" (size) : "d" (port)
    );
}*/

// ====================================================================
void _asm_nop() {       __asm__ __volatile__ ( "nop" : : ); }
void _asm_hlt() {       __asm__ __volatile__ ( "hlt" : : ); }
void _asm_pause() {     __asm__ __volatile__ ( "pause" : : ); } // SSE-2
void _asm_rep_nop() {   __asm__ __volatile__ ( "rep\n\tnop" : : ); }

// ====================================================================
void sleep( uint32_t sec ) {

    uint64_t x = 0; uint64_t i = 0;

    while ( x < sec*10000000*20 ) {

        i = 0;
        while ( i < 1000000000) i++;
        x++;
        //_asm_nop();
        __asm__ __volatile__ ( "nop" : : );
    }


}

void usleep( uint32_t u_sec ) {

    uint64_t x = 0; uint64_t i = 0;

    while ( x < u_sec*10000*20 ) {

        i = 0;
        while ( i < 1000000000) i++;
        x++;
        //_asm_nop();
        __asm__ __volatile__ ( "nop" : : );
    }


}

// ====================================================================
/*void kernel_log( void * _str ) {
    // TODO: data logging
    //kernel_log("");
}
*/

void kernel_log_info( void * _str ) {
    k_term_color = K_TERM_L_BLUE; k_term_print( "[info ]: " );
    k_term_color = K_TERM_WHITE;  k_term_printnl( _str );
    //usleep(25);
    usleep(KERNEL_INFO_DELAY);

}

void kernel_log_error( void * _str ) {
    k_term_color = K_TERM_L_RED; k_term_print( "[error]: " );
    k_term_color = K_TERM_WHITE; k_term_printnl( _str );
    usleep(1000);
    //usleep(KERNEL_INFO_DELAY);

}

void kernel_log_std( void * _str ) {
    k_term_color = K_TERM_WHITE; k_term_print( "[ +++ ]: " );
    k_term_color = K_TERM_WHITE; k_term_printnl( _str );
    //usleep(25);
    usleep(KERNEL_INFO_DELAY);

}

void kernel_log_ok( void * _str ) {
    k_term_color = K_TERM_L_GREEN; k_term_print( "[ ok  ]: " );
    k_term_color = K_TERM_WHITE;   k_term_printnl( _str );
    //usleep(25);
    usleep(KERNEL_INFO_DELAY);

}

// ====================================================================
void system_halt() {

    k_term_clear();

    kernel_log_info(" KERNEL: SYSTME: >>  \n");
    k_term_print(" SYSTEM: HALT: in ");

    k_term_print("3 "); usleep(500);
    k_term_print("2 "); usleep(500);
    k_term_print("1 "); usleep(1000);

    k_term_print("\npower off now \n\n");
    _asm_hlt();

}

// ====================================================================
void system_reboot() {

    k_term_clear();
    kernel_log_info(" KERNEL: SYSTME: >>  \n");
    k_term_print(" SYSTEM: REBOOT: IN ");

    k_term_print("3 "); usleep(200);
    k_term_print("2 "); usleep(200);
    k_term_print("1 "); usleep(500);

    k_term_print("\nreboot now \n\n");


    //__asm__ __volatile__ ( "jmp start" );
    //__asm__ __volatile__ ( "ret" );
    //__asm__ __volatile__ ( "jmp _restart" );

}



// ====================================================================

/*The following array is taken from
http://www.osdever.net/bkerndev/Docs/keyboard.htm
All credits where due*/

uint8_t keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b', /* Backspace */
    '\t',         /* Tab */
    'q', 'w', 'e', 'r',   /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`',   0,        /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
    'm', ',', '.', '/',   0,              /* Right shift */
    '*',
    0,  /* Alt */
    ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};
