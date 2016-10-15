# [cxx] -fno-use-cxa-atexit 	<< memory managment
# [c  ] -nostdlib 				<< libraries, glibc
# [c  ] -fno-builtin			<< 
# [cxx] -fno-rtti 				<< -fno-rtti Disable generation of information about every class with virtual functions for use by the C++ runtime type identification features (dynamic_cast' andtypeid'). If you don't use those parts of the language, you can save some space by using this flag.
# [c  ] -fno-exceptions 		<< 
# [c  ] -fno-leading-underscore << naming of the ASM sections

GCC_PARAMD = -m32 -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore
AS_PARAMD = --32
LD_PARAMS = -melf_i386
OBJECTS = loader.o kernel.o

ROOT = ./
BOOT = $(ROOT)boot/
GRUB = $(BOOT)grub/

%.o: %.c
	gcc $(GCC_PARAMD) -o $@ -c $<

%.o: %.s
	as $(AS_PARAMD) -o $@ -c $<

kernel.bin: linker.ld $(OBJECTS)
	ld $(LD_PARAMS) -T $< -o $@ $(OBJECTS)

intstall: kernel.bin
	cp $< $(BOOT)kernel.bin

clean:
	rm -f *.o && rm -f *.bin
