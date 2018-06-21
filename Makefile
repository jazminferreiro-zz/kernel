#CC=clang
#CPPFLAGS := -nostdlibinc -idirafter lib

CPPFLAGS := -ffreestanding -fno-stack-protector -nostdinc -idirafter lib
GCC_PATH := /usr/lib/gcc/x86_64-pc-linux-gnu/8.1.1
CPPFLAGS += -I$(GCC_PATH)/include -I$(GCC_PATH)/include-fixed

CFLAGS := -g -m32 -O1

QEMU := qemu-system-i386 -serial mon:stdio
KERN := kernel
BOOT := -kernel $(KERN) $(QEMU_EXTRA)

qemu: $(KERN)
	$(QEMU) $(BOOT)

qemu-gdb: $(KERN)
	$(QEMU) -d guest_errors -S -gdb tcp:127.0.0.1:7508 $(BOOT)

gdb:
	gdb -q -s kernel -n -ex 'target remote 127.0.0.1:7508'

kernel: boot.o decls.o kern2-swap.o lib/string.o stacks.o tasks.o func.o contador.o idt_entry.o interrupts.o
	ld -m elf_i386 -Ttext 0x100000  $^ -o $@
	grub-file --is-x86-multiboot $@

%.o: %.S
	$(CC) $(CFLAGS)  $(CPPFLAGS) -c $<
	#$(CC) $(CFLAGS) -c $<

clean:
	rm -f kernel *.o core

.PHONY: clean qemu qemu-gdb gdb
