# ---------------------------------------------------------------------
# Declare constants for the multiboot header.
.set ALIGN,    1<<0                 # align loaded modules on page boundaries
.set MEMINFO,  1<<1                 # provide memory map
.set MAGIC,    0x1BADB002           # 'magic number' lets bootloader find the header
.set FLAGS,    ALIGN | MEMINFO      # this is the Multiboot 'flag' field
.set CHECKSUM, -(MAGIC + FLAGS)     # checksum of above, to prove we are multiboot

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

# ---------------------------------------------------------------------
.section .text
.extern kernel_main
.global loader

loader:
    mov $kernel_stack, %esp
    push %eax                       # info (ram-size, ... ) [multiboot structure]
    push %ebx                       # MAGIC number

    call kernel_main

_end:
    cli
    hlt
    jmp _end

# ======================================================================
# .global _restart
# _restart:
# .global kernel_mknot
# .extern kernel_mknot

# .global kernel_mknot_2
# .extern kernel_mknot_2

# .global test
# .extern test


# ================================ GDT =================================
GP:    .space    64
    .global    GP


.global gdt_flush
gdt_flush:
    lgdt (GP)
    mov $0x10, %ax
    mov  %ax, %ds
    mov  %ax, %es
    mov  %ax, %fs
    mov  %ax, %gs
    mov  %ax, %ss
    # call kernel_mknot
    # jmp $0x08, $flush2 # the opcode is ljmp
    ret
# flush2:
#     ret

# -----------------
#     lgdt (GP)
#     mov ax, 0x10
#     mov ds, ax
#     mov es, ax
#     mov fs, ax
#     mov gs, ax
#     mov ss, ax
#     call kernel_mknot
#     jmp 0x08:flush2
# flush2:
#     ret


# ================================ IDT =================================

IDT_REGISTER:    .space    48
    .global    IDT_REGISTER

.global idt_load
idt_load:
    lidt (IDT_REGISTER)
    # call kernel_mknot
    ret

# ================================ IRQ =================================
.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15

irq0:                   # 32: IRQ0
     cli
     push  $0 
     push  $32
     jmp irq_common_stub

irq1:                   # 33: IRQ1
     cli
     push  $0 
     push  $33
     jmp irq_common_stub

irq2:                   # 34: IRQ2
     cli
     push  $0 
     push  $34
     jmp irq_common_stub

irq3:                   # 35: IRQ3
     cli
     push  $0 
     push  $35
     jmp irq_common_stub

irq4:                   # 36: IRQ4
     cli
     push  $0 
     push  $36
     jmp irq_common_stub

irq5:                   # 37: IRQ5
     cli
     push  $0 
     push  $37
     jmp irq_common_stub

irq6:                   # 38: IRQ6
     cli
     push  $0 
     push  $38
     jmp irq_common_stub

irq7:                   # 39: IRQ7
     cli
     push  $0 
     push  $39
     jmp irq_common_stub

irq8:                   # 40: IRQ8
     cli
     push  $0 
     push  $40
     jmp irq_common_stub

irq9:                   # 41: IRQ9
     cli
     push  $0 
     push  $41
     jmp irq_common_stub

irq10:                  # 42: IRQ10
     cli
     push  $0 
     push  $42
     jmp irq_common_stub

irq11:                  # 43: IRQ11
     cli
     push  $0 
     push  $43
     jmp irq_common_stub

irq12:                  # 44: IRQ12
     cli
     push  $0 
     push  $44
     jmp irq_common_stub

irq13:                  # 45: IRQ13
     cli
     push  $0 
     push  $45
     jmp irq_common_stub

irq14:                  # 46: IRQ14
     cli
     push  $0 
     push  $46
     jmp irq_common_stub

irq15:                  # 47: IRQ15
     cli
     push  $0 
     push  $47
     jmp irq_common_stub


# .extern IRQ_HANDLER
.global IRQ_HANDLER


irq_common_stub:
    # call kernel_mknot
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax

    # ----------------------------------------------
    # push %eax
    # mov (IRQ_HANDLER), %eax # <<<<<<<<<<<<<<
    # call %eax
    # ---------------------
    call (IRQ_HANDLER)
    # ----------------------------------------------


    pop %eax

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret

# =====================================================================
.section .bss
.space 2*1024*1024                  # preserve 2MiB space for kernel
kernel_stack:
