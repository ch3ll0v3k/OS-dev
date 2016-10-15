# ---------------------------------------------------------------------
# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

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
    push %eax # info (ram-size, ... ) [multiboot structure]
    push %ebx # MAGIC number

    call kernel_main



_end:
    cli
    hlt
    jmp _end



.section .bss
.space 2*1024*1024 # preserve 2MiB space for kernel
kernel_stack:



