#include "h/kernel.h"

// ====================================================================

// ====================================================================
// ["HARDWARE-GENERATED EXCEPTIONS"]

// All INT_NUM between 0x0 and 0x1F, inclusive, are reserved for exceptions;
// INT_NUM bigger than 0x1F are used for interrupt routines.
// (Note that the IBM PC did not always obey this rule,
// for instance using interrupt 5 to indicate the Print Screen key was pressed.)

void isr0( /*0x00*/ ) {  printf(" interrupt: ['0x00']: Division by zero\n"); _asm_hlt(); }
void isr1( /*0x01*/ ) {  printf(" interrupt: ['0x01']: Debugger\n"); _asm_hlt(); }
void isr2( /*0x02*/ ) {  printf(" interrupt: ['0x02']: NMI\n"); _asm_hlt(); }
void isr3( /*0x03*/ ) {  printf(" interrupt: ['0x03']: Breakpoint\n"); _asm_hlt(); }
void isr4( /*0x04*/ ) {  printf(" interrupt: ['0x04']: Overflow\n"); _asm_hlt(); }
void isr5( /*0x05*/ ) {  printf(" interrupt: ['0x05']: Bounds\n"); _asm_hlt(); }
void isr6( /*0x06*/ ) {  printf(" interrupt: ['0x06']: Invalid Opcode\n"); _asm_hlt(); }
void isr7( /*0x07*/ ) {  printf(" interrupt: ['0x07']: Coprocessor not available\n"); _asm_hlt(); }
void isr8( /*0x08*/ ) {  printf(" interrupt: ['0x08']: Double fault\n"); _asm_hlt(); }
void isr9( /*0x09*/ ) {  printf(" interrupt: ['0x09']: Coprocessor Segment Overrun (386 or earlier only*\n"); _asm_hlt(); }
void isr10( /*0x0A*/ ) {  printf(" interrupt: ['0x0A']: Invalid Task State Segment\n"); _asm_hlt(); }
void isr11( /*0x0B*/ ) {  printf(" interrupt: ['0x0B']: Segment not present\n"); _asm_hlt(); }
void isr12( /*0x0C*/ ) {  printf(" interrupt: ['0x0C']: Stack Fault\n"); _asm_hlt(); }
void isr13( /*0x0D*/ ) {  printf(" interrupt: ['0x0D']: General protection fault\n"); _asm_hlt(); }
void isr14( /*0x0E*/ ) {  printf(" interrupt: ['0x0E']: Page fault\n"); _asm_hlt(); }
void isr15( /*0x0F*/ ) {  printf(" interrupt: ['0x0F']: reserved\n"); _asm_hlt(); }
void isr16( /*0x10*/ ) {  printf(" interrupt: ['0x10']: Math Fault\n"); _asm_hlt(); }
void isr17( /*0x11*/ ) {  printf(" interrupt: ['0x11']: Alignment Check\n"); _asm_hlt(); }
void isr18( /*0x12*/ ) {  printf(" interrupt: ['0x12']: Machine Check\n"); _asm_hlt(); }
void isr19( /*0x13*/ ) {  printf(" interrupt: ['0x13']: SIMD Floating-Point Exception\n"); _asm_hlt(); }
void isr20( /*0x14*/ ) {  printf(" interrupt: ['0x14']: Virtualization Exception\n"); _asm_hlt(); }
void isr21( /*0x15*/ ) {  printf(" interrupt: ['0x15']: Control Protection Exception\n"); _asm_hlt(); }


void isrALL() {  printf(" INTERRUPT: ['ALL']\n"); _asm_hlt(); }


// ====================================================================

