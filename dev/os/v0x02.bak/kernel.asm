bits	32

section .text
	align	4                       ; align at 4 byte
	dd		0x1BADB002              ; MAGIC
	dd		0x00
	dd		- (0x1BADB002 + 0x00)

global start
extern kernel_main
start:
	cli			; clear interrupts
	push eax	; multiboot structure
	push ebx	; MAGIC

	call kernel_main ; controll to kernel_main(); in kernel_c.c 
	hlt			; pause CPU

end:
	cli
	hlt
	jmp end


