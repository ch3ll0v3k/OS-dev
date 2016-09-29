#include "h/kernel.h"

// ====================================================================
void test_print_func() {
    uint32_t *p_asm_print = (uint32_t *) &_asm_print;
    __asm__ __volatile__ ( "call *%0" : : "r" (p_asm_print) );
    // >>>> test_print_func();

}

void _asm_print() {
    printf( "__asm__ print()\n" );

}



// ====================================================================
void help() {

    //k_term_clear();

    uint16_t i;
    i = 0;

    k_term_print("------------------------------------------------\n" );
    while ( i < k_term_cmd_ttl) {
        printf("[%d] : %s\n", i, k_term_cmds[ i ] );

        i++;

    }

}

// ====================================================================

uint32_t k_term_x_pos = 2;
uint32_t k_term_y_pos = 0;

char *k_term_main_buff[ TERM_MAX_BYTES * 10 ];

void k_term_clear() {

    k_term_color = K_TERM_WHITE;
    char * VID_BUFF = (char *) 0xb8000;
    int c = 0;

    // while ( c < (TERM_W * (TERM_H-1) * 2) ); << OK

    while ( c < (TERM_W * (TERM_H) * 2) ) {
        VID_BUFF[ c++ ] = ' ';
        VID_BUFF[ c++ ] = 0x7;

    }

    k_term_x_pos = 2;
    k_term_y_pos = 0;

    kernel_print_info();
    k_term_update_cursor();

}

// ====================================================================
void k_term_update_cursor() {
    unsigned tmp;

    tmp = (TERM_W * k_term_y_pos + k_term_x_pos)/2;

    outb( 0x3d4, 14 );          // CTR: controll register: Select Cursor Location
    outb( 0x3d5, tmp >> 8 );    // Send the high bytea cross the bus

    outb( 0x3d4, 15 );          // CTR: controll register: Select Send Low byty
    outb( 0x3d5, tmp & 0xFF );         // Send the Low byte or cursor Location

    // see this page
    // https://github.com/ch3ll0v3k/oszur11-operating-system-examples/blob/master/Chapter_03_Interrupts/00_Start/arch/i386/arch/devices/vga_text.c
    // outb ( 0x3D5, t & 0xFF );

}

// ====================================================================
void k_term_clear_line( uint8_t line ) {

    if (line == 0) line = 1;
    line++;

    uint16_t x = 0;
    char * VID_BUFF = (char *) 0xb8000;

    uint32_t LL = (line * TERM_WD);

    while ( x < TERM_WD ) {
        VID_BUFF[ LL + x++ ] = 0x0;
        VID_BUFF[ LL + x++ ] = 0x0;

    }

}

// ====================================================================
void k_term_check_height() {
    if ( k_term_y_pos >= TERM_W * ( TERM_H - 1) * 2 ) {
        k_term_scroll_up();
        k_term_y_pos -= TERM_WD;

    }

}

// ====================================================================
void k_term_scroll_up() {

    uint16_t x = 0;
    uint8_t y = 1;

    char * VID_BUFF = (char *) 0xb8000;

    while ( y < TERM_H ) {

        k_term_clear_line( y );
        y++;

        x = 0;
        while ( x < TERM_WD ) {
            VID_BUFF[ (y * TERM_WD) + x ] = VID_BUFF[ ((y+1) * TERM_WD) + x ];
            x += 1;
        }

    }

}

void k_term_scroll_down() { }


// ====================================================================
void k_term_printnl( void *_str ) {

    k_term_print( _str );
    k_term_print( " \n" );

}

void k_term_print( void *_str) {

    char *str = (char *) _str;
    uint8_t str_c = 0;

    char * VID_BUFF = (char *) 0xb8000;

    while ( str[ str_c ] ) {

        VID_BUFF[ k_term_y_pos + k_term_x_pos ] = str[ str_c ];
        VID_BUFF[ k_term_y_pos + k_term_x_pos +1 ] = k_term_color;

        // if ( str[ str_c ] == 0x08 ) // back_space
        // if ( str[ str_c ] == 0x09 ) // tab

        if ( str[ str_c ] == '\r' ) {
            k_term_x_pos = 2;
        }

        if ( str[ str_c ] == '\n' || k_term_x_pos >= TERM_WD-2 ) {

            /*if ( str[ str_c ] == '\n' ) {
                VID_BUFF[ k_term_y_pos + k_term_x_pos ] = '\0';
                k_term_y_pos += TERM_WD;
                k_term_x_pos = 2;
            } else if (k_term_x_pos >= TERM_WD-2) {

            } */

            VID_BUFF[ k_term_y_pos + k_term_x_pos ] = '\0';
            k_term_y_pos += TERM_WD;
            k_term_x_pos = 2;



        } else {
            k_term_x_pos += 2;

        }

        str_c++;

    }

    k_term_check_height();
    k_term_update_cursor();

}

// ====================================================================
void k_term_print_intnl( int _int ) {

    k_term_print_int( _int );
    k_term_print( " \n" );

}

void k_term_print_int( int _int ) {

    char *VID_BUFF = (char *) 0xb8000;

    char itoa_buff[50];
    itoa( itoa_buff, _int );

    uint16_t len = strlen( itoa_buff );

    uint16_t i = 0;


    while ( i < len ) {
        VID_BUFF[ k_term_y_pos + k_term_x_pos++ ] = itoa_buff[ i++ ];
        VID_BUFF[ k_term_y_pos + k_term_x_pos++ ] = 0x7;

    }

    k_term_check_height();
    k_term_update_cursor();


}

// ====================================================================
/* void put_t( char c, char *buff) {

    // TODO:
    printf("%c", c + key_type); // +49 == UPPER / +81 == LOWER
    buff[ buff_i++ ] = c + key_type;

}*/

// ====================================================================

int dev_by_zero(int i) {

    int b;
    int X = 10;

    for ( ; X >= 0; X-- ) {
        printf("dev_by_zero(%d): \n", X);

        if ( X == 0 ) {
            printf("IS_ZERO_NOW\n");
            usleep(500);
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
void _asm_x21() {
    __asm__ __volatile__(" int $21" : : );
}


void __print() {

    printf("----------------------------------------\n");
    k_term_history_curr_item = 0;

    while ( k_term_history_curr_item < k_term_history_items ) {

        printf("item [%d] [%s]\n", k_term_history_curr_item, k_term_history_map[ k_term_history_curr_item ] );

        k_term_history_curr_item++;
    }

}


void reboot() {
    //__asm__ __volatile__("ljmpw $0xFFFF, $0x0000");
    //__asm__ __volatile__("call haltt");
    //__asm__ __volatile__("call shutdown_n");
    //__asm__ __volatile__("int $19");
    __asm__ __volatile__("jmp start" : : );
}

void k_term_init() {

    // ------------------------------------------------------------
    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setup terminal env");

    k_term_cmd_buff[0] = '\0';
    k_term_cmd_ttl = 0;
    k_term_cmd_c = 0;

    // ------------------------------------------------------------
    k_term_func[ k_term_cmd_ttl ]  = (uint32_t *)  &k_term_clear;
    k_term_cmds[ k_term_cmd_ttl++ ] = "clear";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t *)  &_asm_nop;
    k_term_cmds[ k_term_cmd_ttl++ ] = "ls";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t *)  &_asm_nop;
    k_term_cmds[ k_term_cmd_ttl++ ] = "cd";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t *) &_asm_nop;
    k_term_cmds[ k_term_cmd_ttl++ ] = "exit";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t *) &_asm_nop;
    k_term_cmds[ k_term_cmd_ttl++ ] = "env";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &dev_by_zero;
    k_term_cmds[ k_term_cmd_ttl++ ] = "zero";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &__print;
    k_term_cmds[ k_term_cmd_ttl++ ] = "print";
    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &_asm_x21;
    k_term_cmds[ k_term_cmd_ttl++ ] = "int";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &system_halt;
    k_term_cmds[ k_term_cmd_ttl++ ] = "halt";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &system_reboot;
    k_term_cmds[ k_term_cmd_ttl++ ] = "reboot";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &help;
    k_term_cmds[ k_term_cmd_ttl++ ] = "help";

    // ------------------------------------------------------------
    //k_term_print_intnl( k_term_cmd_ttl );

    k_term_history_items = 0;
    k_term_history_curr_item = 0;

    //printf(" k_term_cmd_c: %d\n", k_term_cmd_c );
    //printf(" k_term_cmd_c: %d\n", k_term_cmd_c );
    //printf(" k_term_cmd_ttl: %d\n", k_term_cmd_ttl );

    //printf(" k_term_history_items: %d\n", k_term_history_items );
    //printf(" k_term_history_curr_item: %d\n", k_term_history_curr_item );

    //k_term_clear();

    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setup terminal env: done");
    // ------------------------------------------------------------

}

// ====================================================================
void reset_promt() {
    k_term_color = K_TERM_L_GREEN;
    k_term_x_pos = 0;
    printf("term#:/ |" );
    k_term_color = K_TERM_WHITE;

}
// ====================================================================
void k_term_history_add() {

    if ( k_term_history_items < 255) {

        sprintf(
            k_term_history_map[ k_term_history_items++ ], "%s", k_term_cmd_buff );

    } else {
        printf("k_term_history_map: is full\n");
    }

}


void k_term_history_walk( int side ) {

    // ----------------------------------------------------------------
    if ( side < 0 ) {
        if ( k_term_history_curr_item < 255) {
            k_term_history_curr_item++;

        }

    } else {

        if ( k_term_history_curr_item > 0) {
            k_term_history_curr_item--;

        }

    }

    if ( k_term_history_curr_item == k_term_history_items && k_term_history_curr_item != 0 ) {
        k_term_history_curr_item--;
    }

    k_term_x_pos = 18;
    k_term_print("                                                                      "); // MAX
    k_term_x_pos = 18;

    sprintf( k_term_cmd_buff, "%s\0", (char *) k_term_history_map[ k_term_history_curr_item ] );


    k_term_cmd_c = ( strlen(k_term_cmd_buff) )+1;


    // ----------------------------------------------------------------

}

// ====================================================================
uint8_t __br__;
uint8_t k_term_process_input_reading = 1;


void k_term_process_input( REG *reg ) {

    // ------------------------------------------------------------
    // Programmable Interrupt Controller (PIC)
    // The keyboard works through the I/O ports 0x60 and 0x64.
    // Port 0x60 gives the data (pressed key) and port 0x64 gives the status

    // ------------------------------------------------------------
    if ( reg->int_no > 10000 && reg->err_code > 10000 ) return;

    uint32_t inportb_input = 0;

    //k_term_color = K_TERM_L_GREEN;
    //k_term_x_pos = 0;
    //printf("term#:/ " );
    //k_term_color = K_TERM_WHITE;



    k_term_process_input_reading = 1;

    while ( inb(0x64) & 0x1 && k_term_process_input_reading ) {

        reset_promt();
        printf( k_term_cmd_buff );


        // http://www.osdever.net/bkerndev/Docs/keyboard.htm
        /*
        while ( (inb(0x64) & 2) != 0 ) {
            // keyboars is busy
        }
        outb( 0x60, 0xED);

        while ( (inb(0x64) & 2) != 0 ) {
            // keyboars is busy
        }
        outb( 0x60, (uint8_t)0b111 );


        if (inb_input & 0x80) {
            // key release
            //break;
        }

        */
        inportb_input = inb(0x60);
        //printf("rrr\n" );

        switch ( inportb_input ) {
            /* ESCP => 0x27; */
            case (1): /*buff[ 0 ] = '\0'; k_term_process_input_reading = 0;*/ break;

            // a -> $ 16 -> 27
            case (2):   /*printf("&"); */ k_term_cmd_buff[ k_term_cmd_c++ ] = '&'+'\0'; break;
            //case (3): /*printf("é"); */ k_term_cmd_buff[ k_term_cmd_c++ ] = 'é'+'\0'; break;
            case (4):   /*printf("\""); */ k_term_cmd_buff[ k_term_cmd_c++ ] = '"'+'\0'; break;
            case (5):   /*printf("'"); */ k_term_cmd_buff[ k_term_cmd_c++ ] = '\''+'\0'; break;
            case (6):   /*printf("("); */ k_term_cmd_buff[ k_term_cmd_c++ ] = '('+'\0'; break;
            //case (7): /*printf("§"); */ k_term_cmd_buff[ k_term_cmd_c++ ] = '§'+'\0'; break;
            //case (8): /*printf("è"); */ k_term_cmd_buff[ k_term_cmd_c++ ] = 'è'+'\0'; break;
            case (9):   /*printf("!"); */ k_term_cmd_buff[ k_term_cmd_c++ ] = '!'+'\0'; break;
            //case (10): /*printf("ç"); */ k_term_cmd_buff[ k_term_cmd_c++ ] = 'ç'+'\0'; break;
            //case (11): /*printf("à"); */ k_term_cmd_buff[ k_term_cmd_c++ ] = 'à'+'\0'; break;
            case (12):   /*printf(")"); */ k_term_cmd_buff[ k_term_cmd_c++ ] = ')'+'\0'; break;


            case (14): // back_space

                if (k_term_x_pos - 18 > 0) {

                    k_term_cmd_buff[ --k_term_cmd_c ] = ' '+'\0';
                    k_term_x_pos -= 2;
                    k_term_update_cursor();

                }

                break;

            // a -> $ 16 -> 27
            case (16): /*printf("a");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'a'+'\0'; break;
            case (17): /*printf("z");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'z'+'\0'; break;
            case (18): /*printf("e");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'e'+'\0'; break;
            case (19): /*printf("r");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'r'+'\0'; break;
            case (20): /*printf("t");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 't'+'\0'; break;
            case (21): /*printf("y");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'y'+'\0'; break;
            case (22): /*printf("u");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'u'+'\0'; break;
            case (23): /*printf("i");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'i'+'\0'; break;
            case (24): /*printf("o");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'o'+'\0'; break;
            case (25): /*printf("p");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'p'+'\0'; break;
            case (26): /*printf("^");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = '^'+'\0'; break;
            case (27): /*printf("$");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = '$'+'\0'; break;

            case (28): /* ENTER */
                k_term_cmd_buff[ k_term_cmd_c ] = '\0';
                printf("\n");
                k_term_process_input_reading = 0;
                k_term_history_curr_item = k_term_history_items;
                break;

            // q -> % 30 - 40
            case (30): /*printf("q");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'q'+'\0'; break;
            case (31): /*printf("s");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 's'+'\0'; break;
            case (32): /*printf("d");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'd'+'\0'; break;
            case (33): /*printf("f");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'f'+'\0'; break;
            case (34): /*printf("g");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'g'+'\0'; break;
            case (35): /*printf("h");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'h'+'\0'; break;
            case (36): /*printf("j");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'j'+'\0'; break;
            case (37): /*printf("k");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'k'+'\0'; break;
            case (38): /*printf("l");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'l'+'\0'; break;
            case (39): /*printf("m");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'm'+'\0'; break;

            // multi character consts (ù), posible overflow
            //case (40): /*printf("ù");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'ù'+'\0'; break;

            // w -> = / 44 -> 53
            case (44): /*printf("w");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'w'+'\0'; break;
            case (45): /*printf("x");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'x'+'\0'; break;
            case (46): /*printf("c");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'c'+'\0'; break;
            case (47): /*printf("v");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'v'+'\0'; break;
            case (48): /*printf("b");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'b'+'\0'; break;
            case (49): /*printf("n");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = 'n'+'\0'; break;
            case (50): /*printf(",");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = ','+'\0'; break;
            case (51): /*printf(";");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = ';'+'\0'; break;
            case (52): /*printf(":");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = ':'+'\0'; break;
            case (53): /*printf("=");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = '='+'\0'; break;

            case (57): // space
                k_term_cmd_buff[ k_term_cmd_c++ ] = ' '+'\0'; printf(" "); break;
            // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            case (75): /*k_term_print("[L]" );*/ break;
            case (77): /*k_term_print("[R]" );*/ break;
            case (72): /*k_term_print("[U]" );*/ k_term_history_walk(1); break;
            case (80): /*k_term_print("[D]" );*/ k_term_history_walk(-1); break;

            case (13): /*printf("-");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = '-'+'\0'; break;

            // is 13 release case (141): printf("-"); k_term_cmd_buff[ k_term_cmd_c++ ] = '-'; break;

            //case (182): printf("r_shift"); k_term_cmd_buff[ k_term_cmd_c++ ] = '   ++++ '; break;


            default:
                /*
                if ( __br__ > 0 ) {
                    printf(", %d] ", inportb_input );
                    __br__ = 0;
                } else {
                    printf("[%d", inportb_input );
                    __br__ = 1;
                }
                */
                break;

        }

        //} // if inportb(0x64) & 0x1 )

        //usleep( 100 );
        //sleep(1);
        //_asm_nop();
        //_asm_hlt();
        //_asm_pause();

    }

    //printf( k_term_cmd_buff );
    //return;


    if ( k_term_process_input_reading == 0 ) {

        uint8_t found = 0;
        //printf("[0]");

        if ( strcmp( k_term_cmd_buff, "" ) == 0 ) {
            found = 1; // skeep, its just empty enter
            //printf("[0:0]");
            reset_promt();

        } else {

            k_term_cmd_c = 0;

            //reset_promt();

            //printf("k_term_cmd_buff: [%s] => ", k_term_cmd_buff );


            while ( k_term_cmd_c < k_term_cmd_ttl ) {

                char *cmd = (char *) k_term_cmds[ k_term_cmd_c ];

                //printf("cmd: [%s]\n", cmd );


                if ( strcmp( k_term_cmd_buff, cmd ) == 0 ) {



                    found = 1;
                    if ( strcmp( k_term_history_map[ k_term_history_items -1 ], k_term_cmd_buff ) != 0 ) {
                        k_term_history_add();
                    }

                    __asm__ __volatile__ ("call *%0 " : : "r" ( (uint32_t *)k_term_func[ k_term_cmd_c ]));

                    break;

                }



                //printf("[2]");
                k_term_cmd_c++;
            }

            // -----------------------------------------------------
            if ( found == 0 ) {
                printf(" \ncmd not found: [%s]\n", k_term_cmd_buff );
                //k_term_printnl("unknown cmd: [ERROR-HERE]" );
            }

            k_term_cmd_c = 0;
            uint32_t len = strlen( k_term_cmd_buff );
            while ( k_term_cmd_c < len )
                k_term_cmd_buff[k_term_cmd_c++] = ' ';
            k_term_cmd_buff[k_term_cmd_c++] = '\0';

            k_term_cmd_c = 0;
            // -----------------------------------------------------


        }

    }

}

// ====================================================================
void kernel_print_info() {

    //-------------------------------------------------------------

    k_term_color = K_TERM_WHITE;    printf(" [ Sat 24-09-2016 14:55 ] [ " );
    //-------------------------------------------------------------
    k_term_color = K_TERM_L_RED;    k_term_print("tmx.os ");
    k_term_color = K_TERM_L_GREEN;  k_term_print("kernel-v0.0.1 ");
    k_term_color = K_TERM_L_BLUE;   k_term_print("i386-i386 ");
    k_term_color = K_TERM_WHITE;    k_term_print(" ] | ");

    //-------------------------------------------------------------
    //k_term_color = K_TERM_WHITE; printf(" ] " );

    TERM_TICKS_AREA = k_term_x_pos;

    k_term_x_pos = 2;
    k_term_y_pos = TERM_WD;

    printf(" ------------------------------------------------------------------------------ " );

    k_term_x_pos = 2;
    k_term_y_pos = TERM_WD*2;

    //k_term_update_cursor();
    //-------------------------------------------------------------
}

// ====================================================================
unsigned char keyboard_map_to_char( int16_t inportb_input ) {

    /*
        1 = ESC
        & -> backspace 2 -> 14

        tab = 15
        enter = 28
        l_ctrl = 29

        ² = 41
        l_shift = 42


        r_shift -> 54

        l_alt = 56
        l_alt = 57
        shift_lock = 58

        < -> 86
        r_alt = 224
        r_alt, r_ctrl = 224

    */

    unsigned char keyboard_map[128] = {
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

    inportb_input = keyboard_map[ inportb_input ];

    return inportb_input;

}

// ====================================================================
