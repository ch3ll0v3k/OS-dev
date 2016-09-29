global irq_common_stub
;global IRQ_HANDLER

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
    ;mov eax, IRQ_HANDLER
    call eax
    pop eax

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret
