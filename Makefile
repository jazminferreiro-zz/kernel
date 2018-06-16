CC=clang
CPPFLAGS := -nostdlibinc -idirafter lib

#CPPFLAGS := -nostdinc -idirafter lib
#GCC_PATH := /usr/lib/gcc/x86_64-linux-gnu/6
#CPPFLAGS += -I$(GCC_PATH)/include -I$(GCC_PATH)/include-fixed

CFLAGS := -g -m32 -O1

QEMU := qemu-system-i386 -serial mon:stdio
KERN := kernel	
BOOT := -kernel $(KERN) $(QEMU_EXTRA)

qemu: $(KERN)
	$(QEMU) $(BOOT)

qemu-gdb: $(KERN)
	$(QEMU) -kernel kern0 -S -gdb tcp:127.0.0.1:7508 $(BOOT)

gdb:
	gdb -q -s kern0 -n -ex 'target remote 127.0.0.1:7508'

kernel: boot.o decls.o kernel.o lib/string.o
	ld -m elf_i386 -Ttext 0x100000  $^ -o $@
	# Verificar imagen Multiboot v1.
	grub-file --is-x86-multiboot $@

%.o: %.S
	$(CC) $(CFLAGS)  $(CPPFLAGS) -c $<
	#$(CC) $(CFLAGS) -c $<

clean:
	rm -f kernel *.o core

.PHONY: clean qemu qemu-gdb gdb









