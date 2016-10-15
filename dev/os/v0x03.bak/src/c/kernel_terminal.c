#include "h/kernel.h"

// ====================================================================
void test_print_func() {
    uint32 *p_asm_print = (uint32 *) &_asm_print;
    __asm__ __volatile__ ( "call *%0" : : "r" (p_asm_print) );
    // >>>> test_print_func();

}

void _asm_print() {
    printf( "__asm__ print()\n" );

}



// ====================================================================

uint32 k_term_x_pos = 2;
uint32 k_term_y_pos = 0;

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

    outportb( 0x3d4, 14 );          // CTR: controll register: Select Cursor Location
    outportb( 0x3d5, tmp >> 8 );    // Send the high byteacross the bus

    outportb( 0x3d4, 15 );          // CTR: controll register: Select Send Low byty
    outportb( 0x3d5, tmp & 0xFF );         // Send the Low byte or cursor Location

    // see this page
    // https://github.com/ch3ll0v3k/oszur11-operating-system-examples/blob/master/Chapter_03_Interrupts/00_Start/arch/i386/arch/devices/vga_text.c
    // outportb ( 0x3D5, t & 0xFF );

}

// ====================================================================
void k_term_clear_line( uint8 line ) {

    if (line == 0) line = 1;
    line++;

    uint16 x = 0;
    char * VID_BUFF = (char *) 0xb8000;

    uint32 LL = (line * TERM_WD);

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

    uint16 x = 0;
    uint8 y = 1;

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
    uint8 str_c = 0;

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
    uint8 is_positive = 1;

    if ( _int < 0 ) {
        is_positive = 0;
        int pos = 0;
        while ( _int < 0 ) {
            pos++;
            _int++;
        }

        _int += pos;

    }

    char k_term_int_buff[32];
    uint16 k_term_int_buff_c = 0;

    while ( _int > 9 ) {

        k_term_int_buff[ k_term_int_buff_c++ ] = (_int % 10) + '0';
        _int /= 10;

    }

    k_term_int_buff[ k_term_int_buff_c++ ] = _int+'0';
    k_term_int_buff[ k_term_int_buff_c ] = '\0';


    int i = k_term_int_buff_c;
    k_term_int_buff_c = 0;

    if ( is_positive == 0 ) {
        VID_BUFF[ k_term_y_pos + k_term_x_pos ] = '-'; k_term_int_buff_c += 2;
        k_term_x_pos += 2;
    }

    for ( ; i>0; i-- ) {
        VID_BUFF[ k_term_y_pos + k_term_x_pos ] = k_term_int_buff[ i-1 ];
        k_term_x_pos += 2;

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
            _usleep(500);
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



char k_term_cmd_buff[512];
char *k_term_cmds[256];
uint32 *k_term_func[256];

uint32 k_term_cmd_c;
uint32 k_term_cmd_ttl;

char k_term_history_map[256][256];
uint8 k_term_history_items;
uint8 k_term_history_curr_item;


void __print() {

    printf("----------------------------------------\n");
    k_term_history_curr_item = 0;

    while ( k_term_history_curr_item < k_term_history_items ) {

        printf("item [%d] [%s]\n", k_term_history_curr_item, k_term_history_map[ k_term_history_curr_item ] );

        k_term_history_curr_item++;
    }

}



void k_term_init() {

    // ------------------------------------------------------------
    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setup terminal env");

    k_term_cmd_buff[0] = '\0';
    k_term_cmd_ttl = 0;
    k_term_cmd_c = 0;

    // ------------------------------------------------------------
    k_term_func[ k_term_cmd_ttl ]  = (uint32 *)  &k_term_clear;
    k_term_cmds[ k_term_cmd_ttl++ ] = "clear";

    k_term_func[ k_term_cmd_ttl ]  = (uint32 *)  &_asm_nop;
    k_term_cmds[ k_term_cmd_ttl++ ] = "ls";

    k_term_func[ k_term_cmd_ttl ]  = (uint32 *)  &_asm_nop;
    k_term_cmds[ k_term_cmd_ttl++ ] = "cd";

    k_term_func[ k_term_cmd_ttl ]  = (uint32 *) &_asm_nop;
    k_term_cmds[ k_term_cmd_ttl++ ] = "exit";

    k_term_func[ k_term_cmd_ttl ]  = (uint32 *) &_asm_nop;
    k_term_cmds[ k_term_cmd_ttl++ ] = "env";

    k_term_func[ k_term_cmd_ttl ]  = (uint32*)  &dev_by_zero;
    k_term_cmds[ k_term_cmd_ttl++ ] = "zero";

    k_term_func[ k_term_cmd_ttl ]  = (uint32*)  &system_halt;
    k_term_cmds[ k_term_cmd_ttl++ ] = "halt";

    k_term_func[ k_term_cmd_ttl ]  = (uint32*)  &__print;
    k_term_cmds[ k_term_cmd_ttl++ ] = "print";

    k_term_func[ k_term_cmd_ttl ]  = (uint32*)  &_asm_x21;
    k_term_cmds[ k_term_cmd_ttl++ ] = "int";

    // ------------------------------------------------------------
    //k_term_print_intnl( k_term_cmd_ttl );

    k_term_history_items = 0;
    k_term_history_curr_item = 0;

    printf(" k_term_cmd_c: %d\n", k_term_cmd_c );
    printf(" k_term_cmd_c: %d\n", k_term_cmd_c );
    printf(" k_term_cmd_ttl: %d\n", k_term_cmd_ttl );

    printf(" k_term_history_items: %d\n", k_term_history_items );
    printf(" k_term_history_curr_item: %d\n", k_term_history_curr_item );




    //k_term_clear();

    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setup terminal env: done");
    // ------------------------------------------------------------

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
    //printf("k_term_history_walk: start\n");

    if ( k_term_history_curr_item == k_term_history_items && k_term_history_curr_item != 0 ) {
        k_term_history_curr_item--;
    }

    k_term_x_pos = 18;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|16+++++++++++++|80
    k_term_print("                                                                      "); // MAX
    //k_term_print("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"); // MAX
    k_term_x_pos = 18;


    k_term_color = K_TERM_L_RED;
    //k_term_print( k_term_history_map[ k_term_history_curr_item ] );
    printf("%d: %s", k_term_history_curr_item ,k_term_history_map[ k_term_history_curr_item ] );
    k_term_color = K_TERM_WHITE;



    sprintf( k_term_cmd_buff, "%s\0", (char *) k_term_history_map[ k_term_history_curr_item ] );


    k_term_cmd_c = ( strlen(k_term_cmd_buff) )+1;

    //printf("\n%s\n", k_term_cmd_buff);


    if ( side < 0 ) {
        if ( k_term_history_curr_item < 255) {
            k_term_history_curr_item++;

        }

    } else {

        if ( k_term_history_curr_item > 0) {
            k_term_history_curr_item--;

        }

    }

    //printf("k_term_history_walk: end\n");
    // ----------------------------------------------------------------

}

// ====================================================================
uint8 __br__;
void k_term_get_input( char *_out ) {

    // ------------------------------------------------------------
    // Programmable Interrupt Controller (PIC)

    // The keyboard works through the I/O ports 0x60 and 0x64.
    // Port 0x60 gives the data (pressed key) and port 0x64 gives the status
    // ------------------------------------------------------------
    // a = 97
    // A = 65

    //uint8 key_type = 81; // ipper case
    //uint8 key_type = 49; // lower case

    _out[0] = '\0';

    k_term_cmd_buff[0] = '\0';
    k_term_cmd_c = 0;

    uint8 reading = 1;

    uint32 inportb_input = 0;

    k_term_color = K_TERM_L_GREEN;
    printf("term#:/ " );
    k_term_color = K_TERM_WHITE;

    while ( reading != 0 ) {

        if ( inportb(0x64) & 0x1 ) {

            inportb_input = inportb(0x60);

            //k_term_printnl(" >>> ");

            switch ( inportb_input ) {
                /* ESCP => 0x27; */
                case (1): /*buff[ 0 ] = '\0'; reading = 0;*/ break;

                // a -> $ 16 -> 27
                case (2): printf("&"); k_term_cmd_buff[ k_term_cmd_c++ ] = '&'; break;
                //case (3): printf("é"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'é'; break;
                case (4): printf("\""); k_term_cmd_buff[ k_term_cmd_c++ ] = '"'; break;
                case (5): printf("'"); k_term_cmd_buff[ k_term_cmd_c++ ] = '\''; break;
                case (6): printf("("); k_term_cmd_buff[ k_term_cmd_c++ ] = '('; break;
                //case (7): printf("§"); k_term_cmd_buff[ k_term_cmd_c++ ] = '§'; break;
                //case (8): printf("è"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'è'; break;
                case (9): printf("!"); k_term_cmd_buff[ k_term_cmd_c++ ] = '!'; break;
                //case (10): printf("ç"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'ç'; break;
                //case (11): printf("à"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'à'; break;
                case (12): printf(")"); k_term_cmd_buff[ k_term_cmd_c++ ] = ')'; break;


                case (14): // back_space

                    if (k_term_x_pos - 18 > 0) {
                        k_term_cmd_buff[ --k_term_cmd_c ] = '\0';
                        k_term_x_pos -= 2;
                        printf("  "); // [\s\s]
                        k_term_x_pos -= 4;
                        k_term_update_cursor();
                    }

                    break;

                // a -> $ 16 -> 27
                case (16): printf("a"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'a'; break;
                case (17): printf("z"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'z'; break;
                case (18): printf("e"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'e'; break;
                case (19): printf("r"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'r'; break;
                case (20): printf("t"); k_term_cmd_buff[ k_term_cmd_c++ ] = 't'; break;
                case (21): printf("y"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'y'; break;
                case (22): printf("u"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'u'; break;
                case (23): printf("i"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'i'; break;
                case (24): printf("o"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'o'; break;
                case (25): printf("p"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'p'; break;
                case (26): printf("^"); k_term_cmd_buff[ k_term_cmd_c++ ] = '^'; break;
                case (27): printf("$"); k_term_cmd_buff[ k_term_cmd_c++ ] = '$'; break;

                case (28): /* ENTER */
                    k_term_cmd_buff[ k_term_cmd_c ] = '\0';
                    printf("\n");
                    reading = k_term_cmd_c = 0;
                    k_term_history_curr_item = k_term_history_items;
                    break;

                // q -> % 30 - 40
                case (30): printf("q"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'q'; break;
                case (31): printf("s"); k_term_cmd_buff[ k_term_cmd_c++ ] = 's'; break;
                case (32): printf("d"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'd'; break;
                case (33): printf("f"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'f'; break;
                case (34): printf("g"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'g'; break;
                case (35): printf("h"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'h'; break;
                case (36): printf("j"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'j'; break;
                case (37): printf("k"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'k'; break;
                case (38): printf("l"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'l'; break;
                case (39): printf("m"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'm'; break;

                // multi character consts (ù), posible overflow
                //case (40): printf("ù"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'ù'; break;

                // w -> = / 44 -> 53
                case (44): printf("w"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'w'; break;
                case (45): printf("x"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'x'; break;
                case (46): printf("c"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'c'; break;
                case (47): printf("v"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'v'; break;
                case (48): printf("b"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'b'; break;
                case (49): printf("n"); k_term_cmd_buff[ k_term_cmd_c++ ] = 'n'; break;
                case (50): printf(","); k_term_cmd_buff[ k_term_cmd_c++ ] = ','; break;
                case (51): printf(";"); k_term_cmd_buff[ k_term_cmd_c++ ] = ';'; break;
                case (52): printf(":"); k_term_cmd_buff[ k_term_cmd_c++ ] = ':'; break;
                case (53): printf("="); k_term_cmd_buff[ k_term_cmd_c++ ] = '='; break;

                case (57): // space
                    k_term_cmd_buff[ k_term_cmd_c++ ] = ' '; printf(" "); break;

                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                case (75): /*k_term_print("[L]" );*/ break;
                case (77): /*k_term_print("[R]" );*/ break;
                case (72): /*k_term_print("[U]" );*/ k_term_history_walk(1); break;
                case (80): /*k_term_print("[D]" );*/ k_term_history_walk(-1); break;

                case (13): printf("-"); k_term_cmd_buff[ k_term_cmd_c++ ] = '-'; break;

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

        } // if

        //_usleep( 100 );
        //_sleep(1);
        //_asm_nop();
        //_asm_hlt();
        //_asm_pause();

    } // while( reading )


    uint8 found = 0;
    //printf("[0]");


    if ( strcmp( k_term_cmd_buff, "" ) == 0 ) {
        found = 1; // skeep, its just empty enter
        //printf("[0:0]");


    } else {

        //printf("[1]");
        k_term_cmd_c = 0;


        int test = k_term_cmd_ttl;

        if ( test < 0 )
            k_term_cmd_ttl = 0;

        while ( k_term_cmd_c < k_term_cmd_ttl ) {

            //printf("[1....]");
            //printf("k_term_cmd_buff: [%s] | \n", k_term_cmd_buff );

            //printf("%d of %d \n", k_term_cmd_c, k_term_cmd_ttl );

            char *cmd = (char *) k_term_cmds[ k_term_cmd_c ];
            if ( strcmp( k_term_cmd_buff, cmd ) == 0 ) {

                //printf("[1....1]");

                found = 1;
                if ( strcmp( k_term_history_map[ k_term_history_items -1 ], k_term_cmd_buff ) != 0 ) {
                    k_term_history_add();
                }

                k_term_cmd_buff[0] = '\0';

                __asm__ __volatile__ ("call *%0 " : : "r" ( (uint32 *)k_term_func[ k_term_cmd_c ]));

                break;

            }

            //printf("[2]");
            k_term_cmd_c++;
        }
        /**/
    }

    //printf("[3]");
    if ( found == 0 ) {
        printf("unknown cmd: [%s]\n", k_term_cmd_buff );
        //k_term_printnl("unknown cmd: [ERROR-HERE]" );
        k_term_cmd_buff[0] = '\0';
    }

}

// ====================================================================
void kernel_print_info() {

    //-------------------------------------------------------------

    k_term_color = K_TERM_WHITE;
    printf(" [ Sat 24-09-2016 14:55 ] [ " );
    //-------------------------------------------------------------
    k_term_color = K_TERM_L_RED;
    k_term_print("tmx.os ");

    k_term_color = K_TERM_L_GREEN;
    k_term_print("kernel-v0.0.1 ");

    k_term_color = K_TERM_L_BLUE;
    k_term_print("i386-i386 ");

    //-------------------------------------------------------------
    k_term_color = K_TERM_WHITE;
    printf(" ] " );
    k_term_x_pos = 2;
    k_term_y_pos = TERM_WD;

    printf(" ------------------------------------------------------------------------------ " );

    k_term_x_pos = 2;
    k_term_y_pos = TERM_WD*2;

    //k_term_update_cursor();
    //-------------------------------------------------------------
}

// ====================================================================
unsigned char keyboard_map_to_char( int16 inportb_input ) {

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
