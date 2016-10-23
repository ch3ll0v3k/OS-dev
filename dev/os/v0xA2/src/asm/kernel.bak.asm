bits	32
; // ======================================================================
; // https://0xax.gitbooks.io/linux-insides/content/Booting/linux-bootstrap-1.html 
; // 0x00000000 - 0x000003FF - Real Mode Interrupt Vector Table
; // 0x00000400 - 0x000004FF - BIOS Data Area
; // 0x00000500 - 0x00007BFF - Unused
; // 0x00007C00 - 0x00007DFF - Our Bootloader
; // 0x00007E00 - 0x0009FFFF - Unused
; // 0x000A0000 - 0x000BFFFF - Video RAM (VRAM) Memory
; // 0x000B0000 - 0x000B7777 - Monochrome Video Memory
; // 0x000B8000 - 0x000BFFFF - Color Video Memory
; // 0x000C0000 - 0x000C7FFF - Video ROM BIOS
; // 0x000C8000 - 0x000EFFFF - BIOS Shadow Area
; // 0x000F0000 - 0x000FFFFF - System BIOS

; // ======================================================================
; // MODE    RESOLUTION  BITS PER PIXEL  MAXIMUM COLORS
; // 0x0100  640x480     4               16
; // 0x0101  640x480     8               256
; // 0x0102  800x600     4               16
; // 0x0103  800x600     8               256
; // 0x010D  320x200     15              32k
; // 0x010E  320x200     16              64k
; // 0x010F  320x200     24/32*          16m
; // 0x0110  640x480     15              32k
; // 0x0111  640x480     16              64k
; // 0x0112  640x480     24/32*          16m
; // 0x0113  800x600     15              32k
; // 0x0114  800x600     16              64k
; // 0x0115  800x600     24/32*          16m
; // 0x0116  1024x768    15              32k
; // 0x0117  1024x768    16              64k
; // 0x0118  1024x768    24/32*          16m

; // mov ax, 0x4F02	; // set VBE mode
; // mov bx, 0x4118	; // VBE mode number; notice that bits 0-13 contain the mode number and bit 14 (LFB) is set and bit 15 (DM) is clear.
; // int 0x10		; // call VBE BIOS
; // cmp ax, 0x004F	; // test for error
; // jne error

;//======================================================================
MBALIGN  equ  1<<0              ; // align loaded modules on page boundaries
MEMINFO  equ  1<<1              ; // provide memory map
VIDMOD   equ  1<<2              ; // information about the video mode table must be available to the kernel.
FLAGS    equ  MBALIGN | MEMINFO | VIDMOD
MAGIC    equ  0x1BADB002        ; // 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; // checksum of above, to prove we are multiboot
 
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
   	dd 0						; 12 	u32 	header_addr 	if flags[16] is set
    dd 0						; 16 	u32 	load_addr 	if flags[16] is set
    dd 0						; 20 	u32 	load_end_addr 	if flags[16] is set
    dd 0						; 24 	u32 	bss_end_addr 	if flags[16] is set
    dd 0						; 28 	u32 	entry_addr 	if flags[16] is set
    dd 0						; 32 	u32 	mode_type 	if flags[2] is set
    dd 1024						; 36 	u32 	width 	if flags[2] is set
    dd 768						; 40 	u32 	height 	if flags[2] is set
    dd 32						; 44 	u32 	depth 	if flags[2] is set 


; // MULTIBOOT_HEADER_MAGIC                  0x1BADB002
; // MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

;//======================================================================
global loader
extern kernel_main

loader:

	cli						; Clear interrupt flag [IF = 0]; 0xFA
	mov esp, kernel_stack	; set stack pointer
	push ebx				; info (ram-size, ... ) [multiboot structure]
	push eax				; MAGIC
	call kernel_main 		; controll to kernel_main(); in kernelc.c 
	hlt						; HLT 	Enter halt state 0xF4

end:
	cli
	hlt
	jmp end

;//======================================================================
global _restart
_restart:
global kernel_mknot
extern kernel_mknot

global kernel_mknot_2
extern kernel_mknot_2

global test
extern test

;//======================================================================
;//================================ GDT =================================
global gdt_flush
extern GP
gdt_flush:
	lgdt [GP]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	call kernel_mknot
	jmp 0x08:flush2
flush2:
	ret


;//================================ IDT =================================
global idt_load
extern IDT_REGISTER
idt_load:
	lidt [IDT_REGISTER]
	call kernel_mknot
	ret

;//================================ IRQ =================================
extern IRQ_HANDLER
global IRQ_HANDLER

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

irq0:                   ; 32: IRQ0
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub

irq1:                   ; 33: IRQ1
	cli
	push byte 0
	push byte 33
	jmp irq_common_stub

irq2:                   ; 34: IRQ2
	cli
	push byte 0
	push byte 34
	jmp irq_common_stub

irq3:                   ; 35: IRQ3
	cli
	push byte 0
	push byte 35
	jmp irq_common_stub

irq4:                   ; 36: IRQ4
	cli
	push byte 0
	push byte 36
	jmp irq_common_stub

irq5:                   ; 37: IRQ5
	cli
	push byte 0
	push byte 37
	jmp irq_common_stub

irq6:                   ; 38: IRQ6
	cli
	push byte 0
	push byte 38
	jmp irq_common_stub

irq7:                   ; 39: IRQ7
	cli
	push byte 0
	push byte 39
	jmp irq_common_stub

irq8:                   ; 40: IRQ8
	cli
	push byte 0
	push byte 40
	jmp irq_common_stub

irq9:                   ; 41: IRQ9
	cli
	push byte 0
	push byte 41
	jmp irq_common_stub

irq10:              ; 42: IRQ10
	cli
	push byte 0
	push byte 42
	jmp irq_common_stub

irq11:              ; 43: IRQ11
	cli
	push byte 0
	push byte 43
	jmp irq_common_stub

irq12:              ; 44: IRQ12
	cli
	push byte 0
	push byte 44
	jmp irq_common_stub

irq13:              ; 45: IRQ13
	cli
	push byte 0
	push byte 45
	jmp irq_common_stub

irq14:              ; 46: IRQ14
	cli
	push byte 0
	push byte 46
	jmp irq_common_stub

irq15:              ; 47: IRQ15
	cli
	push byte 0
	push byte 47
	jmp irq_common_stub

irq_common_stub:
	;call kernel_mknot
	pusha
	push ds
	push es
	push fs
	push gs

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp

	push eax
	mov eax, IRQ_HANDLER
	call eax
	pop eax

	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret

;//======================================================================
global shutdown_n
shutdown_n:
	;//mov ax,  0x1000
	;//mov ss,  ax
	mov sp,   0xf000
	mov ax,   0x5307
	mov bx,   0x0001
	mov cx,   0x0003
	int 15
	ret

global shutdown_nn
shutdown_nn:
	mov  ax,  5307
	mov  bx,  0001
	mov  cx,  0003
	int  0x0F
	ret

global haltt
haltt:

	mov     ax, 5301
	xor     bx, bx
	int     15

	;//try to set apm version (to 1.2)
	mov     ax, 5300
	xor     bx, bx
	mov     cx, 0102
	int     15

	;//turn off the system
	mov     ax, 5307
	mov     bx, 0001
	mov     cx, 0003
	int     15

	;//exit (for good measure and in case of failure)
	ret
;//======================================================================
section .bss
	; // resb 1024*256			; *Kb for stack
	resb 1024*1024*4			; *Kb for stack

kernel_stack: 				; <= stack_space




