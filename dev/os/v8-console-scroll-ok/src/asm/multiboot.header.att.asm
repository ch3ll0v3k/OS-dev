    # multiboot 2 header (see http://download-mirror.savannah.gnu.org/releases/grub/phcoder/multiboot.pdf)
    .balign 8
mbhdr:
    .long 0xe85250d6 # magic
    .long 0 # architecture (i386, 32-bit)
    .long .LhdrEnd-mbhdr # header length
    .long -(.LhdrEnd-mbhdr+0xe85250d6) # checksum
    # tags
    # module align
    .word 6 # type
    .word 0 # flags
    .long 8 # size in bytes (spec says 12?)
    .balign 8
    # loader entry
    .word 3
    .word 0
    .long 12
    .long entry
    .balign 8
    # console flags
    .word 4
    .word 0
    .long 12
    .long 0x03 # EGA text support, require console
    .balign 8
    # info request 
    .word 1
    .word 0
    .long 4*6+8
    .long 5 # BIOS boot device
    .long 1 # command line
    .long 3 # modules
    .long 9 # ELF symbols
    .long 6 # memory map
    .long 10 # APM table
    .balign 8
    # address info
    .word 2 # type
    .word 0 # flags
    .long 24 # size
    .long mbhdr # header load addr
    .long 0x100000 # load addr
    .long 0 # load end addr (entire file)
    .long 0 # BSS end addr (no BSS)
    .balign 8
    # terminating tag
    .word 0
    .word 0
    .long 8
.LhdrEnd: