bits	32

section .text
	align	4                       ; align at 4 byte
	dd		0x1BADB002              ; MAGIC
	dd		0x00
	dd		- (0x1BADB002 + 0x00)

;global gdt_flush
global start

;extern gp
extern kernel_main

;// cli
;// lgdt [gdt_descr
;// lidt [idt_descr
;// jmp 0x08:.1
;// .1


start:

	cli						; Clear interrupt flag [IF = 0]; 0xFA
	mov esp, kernel_stack	; set stack pointer
	;push eax				; multiboot structure
	;push ebx				; MAGIC
	call kernel_main 		; controll to kernel_main(); in kernelc.c 
	hlt						; HLT 	Enter halt state 0xF4

end:
	cli
	hlt
	jmp end

;gdt_flush:
;    lgdt [gp]        ; Load the GDT with our '_gp' which is a special pointer
;    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
;    mov ds, ax
;    mov es, ax
;    mov fs, ax
;    mov gs, ax
;    mov ss, ax
;    jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
;flush2:
;    ret               ; Returns back to the C code!



section .bss
	resb 1024*256			; *Kb for stack
	;resb 1024*512			; *Kb for stack
	kernel_stack: 			; <= stack_space




