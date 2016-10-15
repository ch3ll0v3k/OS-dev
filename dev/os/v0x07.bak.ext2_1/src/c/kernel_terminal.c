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

    while ( c < (TERM_W * (TERM_H-1) * 2) ) {
        VID_BUFF[ c++ ] = ' ';
        VID_BUFF[ c++ ] = 0x0F;

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
        VID_BUFF[ LL + x++ ] = 0x0F;

    }

}

// ====================================================================
void k_term_check_height() {
    if ( k_term_y_pos >= TERM_W * ( TERM_H - 3) * 2 ) {
        k_term_scroll_up();
        k_term_y_pos -= TERM_WD;

    }

}

// ====================================================================
void k_term_scroll_up() {

    uint16_t x = 0;
    uint8_t y = 1;

    char * VID_BUFF = (char *) 0xb8000;

    while ( y < TERM_H-3 ) {

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
    itoa( itoa_buff, _int, 10 );

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
void k_term_status_bar_clear() {

    uint32_t i = 0;

    while ( i < TERM_W ) k_term_status_bar_buff[ i++ ] = ' ';

}

void k_term_status_bar_draw() {

    // ------------------------------------------------------------
    // 32 Kb * 1024 byte = 32768 byte
    // 80 x 25 x 2 = 4000 bytes per screen! you have 8 display pages to use.

    // 0xA0000 for EGA/VGA graphics modes (64 KB)
    // 0xB0000 for monochrome text mode (32 KB)
    // 0xB8000 for color text mode and CGA-compatible graphics modes (32 KB)

    // K_TERM_L_GREEN == 0xa
    // K_TERM_L_BLUE == 0x9
    // K_TERM_L_RED == 0xc
    // K_TERM_WHITE == 0xf
    // ------------------------------------------------------------
    char * BUFF = (char *)(0xb8000 + TERM_WD * (TERM_H-1) );
    uint32_t ii = 0, i = 0;

    while ( i < TERM_W ) {

        BUFF[ii] = ( 0 << 8 );

        BUFF[ii++] = k_term_status_bar_buff[ i ];

        if ( i >= 67 )      BUFF[ii++] = 0x1F;
        else if ( i >= 55 ) BUFF[ii++] = 0x2F;
        else if ( i >= 43 ) BUFF[ii++] = 0x4F;
        else                BUFF[ii++] = 0x8F;


        // if ( i >= 67 )      BUFF[ii++] = 0x10;
        // else if ( i >= 55 ) BUFF[ii++] = 0x20;
        // else if ( i >= 43 ) BUFF[ii++] = 0x40;
        // else                BUFF[ii++] = 0x8F;


        i++;

    }

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
    //__asm__ __volatile__("jmp start" : : );
}

void k_term_init() {

    // ------------------------------------------------------------
    if (KERNEL_BOOT_INFO) kernel_log_info(" :: setup terminal env");

    k_term_cmd_buff[0] = '\0';
    k_term_cmd_ttl = 0;
    k_term_cmd_c = 0;

    // ------------------------------------------------------------
    k_term_func[ k_term_cmd_ttl ]  = (uint32_t *) &k_term_status_bar_draw;
    k_term_cmds[ k_term_cmd_ttl++ ] = "status";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t *) &k_term_clear;
    k_term_cmds[ k_term_cmd_ttl++ ] = "clear";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*) &help;
    k_term_cmds[ k_term_cmd_ttl++ ] = "help";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*) &__print;
    k_term_cmds[ k_term_cmd_ttl++ ] = "print";

    // -------------------------------
    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &get_root_dir;
    k_term_cmds[ k_term_cmd_ttl++ ] = "ls-root";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &ext2_directory;
    k_term_cmds[ k_term_cmd_ttl++ ] = "ls";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &ext2_inode_info;
    k_term_cmds[ k_term_cmd_ttl++ ] = "next-inode";

    // -------------------------------

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &get_0_master_info;
    k_term_cmds[ k_term_cmd_ttl++ ] = "ata0-m";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &get_0_slave_info;
    k_term_cmds[ k_term_cmd_ttl++ ] = "ata0-s";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &get_1_master_info;
    k_term_cmds[ k_term_cmd_ttl++ ] = "ata1-m";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &get_1_slave_info;
    k_term_cmds[ k_term_cmd_ttl++ ] = "ata1-s";





    // -------------------------------
    /** IS FROM [kernel_fat32.c]
    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &fat32_test;
    k_term_cmds[ k_term_cmd_ttl++ ] = "fat";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &LBA_UP;
    k_term_cmds[ k_term_cmd_ttl++ ] = "lbaup";
    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &LBA_DOWN;
    k_term_cmds[ k_term_cmd_ttl++ ] = "lbadown";
    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &LBA_GET;
    k_term_cmds[ k_term_cmd_ttl++ ] = "lbaget";

    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &SEC_UP;
    k_term_cmds[ k_term_cmd_ttl++ ] = "secup";
    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &SEC_DOWN;
    k_term_cmds[ k_term_cmd_ttl++ ] = "secdown";
    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &SEC_GET;
    k_term_cmds[ k_term_cmd_ttl++ ] = "secget";
    */


    k_term_func[ k_term_cmd_ttl ]  = (uint32_t*)  &kernel_mem_info;
    k_term_cmds[ k_term_cmd_ttl++ ] = "multiboot-mem-info";

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

    k_term_status_bar_color = 0xF0;
    sprintf( k_term_status_bar_buff, "  >>> setup terminal env: done" );
    k_term_status_bar_draw();
    // ------------------------------------------------------------

}

// ====================================================================
void reset_promt() {
    k_term_color = K_TERM_L_GREEN;
    k_term_x_pos = 0;
    printf(" term#:/ " );
    k_term_color = 0x0F;

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

uint8_t k_term_SHIFT_L = 0;
uint8_t k_term_SHIFT_R = 0;

uint8_t k_term_CTRL_L = 0;
uint8_t k_term_CTRL_R = 0;

uint8_t k_term_ALT_L = 0;
uint8_t k_term_ALT_GR = 0;

uint32_t inportb_input = 0;
void k_term_process_input( REG *reg ) {

    // ------------------------------------------------------------
    // Programmable Interrupt Controller (PIC)
    // The keyboard works through the I/O ports 0x60 and 0x64.
    // Port 0x60 gives the data (pressed key) and port 0x64 gives the status

    // ------------------------------------------------------------
    if ( reg->int_no > 10000 && reg->err_code > 10000 ) return;


    inportb_input = 0;


    if ( k_term_cmd_c >= 0 ) {

        k_term_status_bar_color = 0xF0;
        sprintf( k_term_status_bar_buff, " x[%d] , c[%d] [MORE]", k_term_x_pos, k_term_cmd_c );

    } else {
        k_term_status_bar_color = 0xCE;
        sprintf( k_term_status_bar_buff, " x[%d] , c[%d] [LESS]", k_term_x_pos, k_term_cmd_c );

    }

    k_term_color = K_TERM_WHITE;


    k_term_status_bar_draw();

    k_term_process_input_reading = 1;

    while ( inb(0x64) & 0x1 && k_term_process_input_reading ) {

        reset_promt();
        printf( k_term_cmd_buff );


        // http://www.osdever.net/bkerndev/Docs/keyboard.htm
        inportb_input = inb(0x60);

        switch ( inportb_input ) {
            /* ESCP => 0x27; */
            case (1): /*buff[ 0 ] = '\0'; k_term_process_input_reading = 0;*/ break;

            // a -> $ 16 -> 27
            case (2): k_term_cmd_buff[ k_term_cmd_c++ ]  = (k_term_ALT_GR) ? '|'+'\0' :(k_term_SHIFT_L) ? '1'+'\0' : '&'+'\0'; break;
            case (3): k_term_cmd_buff[ k_term_cmd_c++ ]  = (k_term_ALT_GR) ? '@'+'\0' : '2'+'\0'; break; //(k_term_SHIFT_L) ? '2'+'\0' : (char)("é"+'\0'); break;
            case (4): k_term_cmd_buff[ k_term_cmd_c++ ]  = (k_term_ALT_GR) ? '#'+'\0' : (k_term_SHIFT_L) ? '3'+'\0' : '"'+'\0'; break;
            case (5): k_term_cmd_buff[ k_term_cmd_c++ ]  = (k_term_SHIFT_L) ? '4'+'\0' : '\''+'\0'; break;
            case (6): k_term_cmd_buff[ k_term_cmd_c++ ]  = (k_term_SHIFT_L) ? '5'+'\0' : '('+'\0'; break;
            case (7): k_term_cmd_buff[ k_term_cmd_c++ ]  = '6'+'\0'; break; //(k_term_SHIFT_L) ? '6'+'\0' : (char)("§"+'\0'); break;
            case (8): k_term_cmd_buff[ k_term_cmd_c++ ]  = '7'+'\0'; break; //(k_term_SHIFT_L) ? '7'+'\0' : (char)("è"+'\0'); break;
            case (9): k_term_cmd_buff[ k_term_cmd_c++ ]  = (k_term_SHIFT_L) ? '8'+'\0' : '!'+'\0'; break;
            case (10): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_ALT_GR) ? '{'+'\0' : '9'+'\0'; break;
            case (11): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_ALT_GR) ? '}'+'\0' : '0'+'\0'; break;
            case (12): k_term_cmd_buff[ k_term_cmd_c++ ] = ')'+'\0'; break; //(k_term_SHIFT_L) ? "°"+'\0' : ')'+'\0'; break;

            case (14): // back_space
                if (k_term_x_pos - 18 > 0) {
                    k_term_cmd_buff[ --k_term_cmd_c ] = ' '+'\0'; k_term_x_pos -= 2;

                    if ( k_term_cmd_c <= 0 ) k_term_cmd_c = 0;
                    if ( k_term_x_pos <= 0 ) k_term_x_pos = 18;


                    k_term_update_cursor();

                }

                break;

            case (15): // [TAB]
                //k_term_cmd_buff[ k_term_cmd_c++ ] = 'a'+'\0';
                break;

            // a -> $ 16 -> 27
            case (16): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'a'-32+'\0' : 'a'+'\0'; break;
            case (17): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'z'-32+'\0' : 'z'+'\0'; break;
            case (18): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'e'-32+'\0' : 'e'+'\0'; break;
            case (19): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'r'-32+'\0' : 'r'+'\0'; break;
            case (20): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 't'-32+'\0' : 't'+'\0'; break;
            case (21): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'y'-32+'\0' : 'y'+'\0'; break;
            case (22): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'u'-32+'\0' : 'u'+'\0'; break;
            case (23): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'i'-32+'\0' : 'i'+'\0'; break;
            case (24): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'o'-32+'\0' : 'o'+'\0'; break;
            case (25): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'p'-32+'\0' : 'p'+'\0'; break;
            case (26): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_ALT_GR) ? '['+'\0' : '^'+'\0'; break;

            case (27): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_ALT_GR) ? '['+'\0' :  (k_term_SHIFT_L) ? '*'-32+'\0' : '$'+'\0'; break;

            case (28): /* ENTER */
                k_term_cmd_buff[ k_term_cmd_c ] = '\0';
                printf("\n");
                k_term_process_input_reading = 0;
                k_term_history_curr_item = k_term_history_items;
                break;

            // q -> % 30 - 40
            case (30): /*printf("q");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'Q'+'\0' : 'q'+'\0'; break;
            case (31): /*printf("s");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'S'+'\0' : 's'+'\0'; break;
            case (32): /*printf("d");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'D'+'\0' : 'd'+'\0'; break;
            case (33): /*printf("f");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'F'+'\0' : 'f'+'\0'; break;
            case (34): /*printf("g");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'G'+'\0' : 'g'+'\0'; break;
            case (35): /*printf("h");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'H'+'\0' : 'h'+'\0'; break;
            case (36): /*printf("j");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'J'+'\0' : 'j'+'\0'; break;
            case (37): /*printf("k");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'K'+'\0' : 'k'+'\0'; break;
            case (38): /*printf("l");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'L'+'\0' : 'l'+'\0'; break;
            case (39): /*printf("m");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'M'+'\0' : 'm'+'\0'; break;

            // multi character consts (ù), posible overflow
            // case (40): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? '£'+'\0' : 'µ'+'\0'; break;

            // w -> = / 44 -> 53
            case (44): /*printf("w");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'W'+'\0' : 'w'+'\0'; break;
            case (45): /*printf("x");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'X'+'\0' : 'x'+'\0'; break;
            case (46): /*printf("c");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'C'+'\0' : 'c'+'\0'; break;
            case (47): /*printf("v");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'V'+'\0' : 'v'+'\0'; break;
            case (48): /*printf("b");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'B'+'\0' : 'b'+'\0'; break;
            case (49): /*printf("n");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? 'N'+'\0' : 'n'+'\0'; break;
            case (50): /*printf(",");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? '?'+'\0' : ','+'\0'; break;
            case (51): /*printf(";");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? '.'+'\0' : ';'+'\0'; break;
            case (52): /*printf(":");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? '/'+'\0' : ':'+'\0'; break;
            case (53): /*printf("=");*/ k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? '+'+'\0' : '='+'\0'; break;

            case (57): // space
                k_term_cmd_buff[ k_term_cmd_c++ ] = ' '+'\0';  break;
            // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            case (75): /*k_term_print("[L]" );*/ break;
            case (77): /*k_term_print("[R]" );*/ break;
            case (72): /*k_term_print("[U]" );*/ k_term_history_walk(1); break;
            case (80): /*k_term_print("[D]" );*/ k_term_history_walk(-1); break;

            case (13): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_SHIFT_L) ? '_'+'\0' : '-'+'\0'; break;

            // is 13 release case (141): printf("-"); k_term_cmd_buff[ k_term_cmd_c++ ] = '-'; break;

            //case (182): printf("r_shift"); k_term_cmd_buff[ k_term_cmd_c++ ] = '   ++++ '; break;

            case (42):  case (54):  k_term_SHIFT_L=1; break;
            case (182): case (170): k_term_SHIFT_L=0; break;

            case (29):  k_term_CTRL_R=1; break;
            case (157): k_term_CTRL_R=0; break;

            case (56):  k_term_ALT_GR=1; break;
            case (184): k_term_ALT_GR=0; break;

            case (86): k_term_cmd_buff[ k_term_cmd_c++ ] = (k_term_ALT_GR) ? '\\'+'\0' :(k_term_SHIFT_L) ? '>'+'\0' : '<'+'\0'; break;

            default:
                /*
                if ( __br__ > 0 ) {
                    // DOWN
                    printf(", %d] ", inportb_input );
                    __br__ = 0;
                } else {
                    // UP
                    printf("[%d", inportb_input );
                    __br__ = 1;
                }
                */
                break;

        }
    } // while


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
                k_term_color = K_TERM_L_RED;
                k_term_print(" \n not found:" );
                k_term_color = K_TERM_L_BLUE;
                k_term_print( "[" );
                k_term_color = K_TERM_L_GREEN;
                k_term_print( "\" " );

                k_term_color = K_TERM_WHITE;
                k_term_print( k_term_cmd_buff );

                k_term_color = K_TERM_L_GREEN;
                k_term_print( " \"" );
                k_term_color = K_TERM_L_BLUE;
                k_term_printnl( "]" );
                k_term_color = 0x0f;


            }

            k_term_cmd_c = 0;
            int32_t len = strlen( k_term_cmd_buff );
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

    k_term_color = K_TERM_WHITE;    printf(" [ NNN 00-00-000 00:00 ] [ " );
    //-------------------------------------------------------------
    k_term_color = K_TERM_L_RED;    k_term_print("tmx.os ");
    k_term_color = K_TERM_L_GREEN;  k_term_print("kernel-v0.0.1 ");
    k_term_color = K_TERM_L_BLUE;   k_term_print("i386-i386 ");
    k_term_color = K_TERM_WHITE;    k_term_print(" ] | ");

    //-------------------------------------------------------------
    //k_term_color = K_TERM_WHITE; printf(" ] " );

    TERM_TICKS_AREA = k_term_x_pos;
    k_term_x_pos = 0;
    k_term_y_pos = TERM_WD;


    k_term_color = 0x0C;
    printf(" ________________________________________________________________________________" );
    k_term_color = K_TERM_WHITE;

    k_term_x_pos = 0;
    k_term_y_pos = TERM_WD*2;

    //k_term_update_cursor();
    //-------------------------------------------------------------
}

// ====================================================================
unsigned char keyboard_map_to_char( int16_t inportb_input ) { return inportb_input; }

// ====================================================================
