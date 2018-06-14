CFLAGS := -g -m32 -O1

kernel: boot.o kernel.o
	ld -m elf_i386 -Ttext 0x100000 --entry ??? $^ -o $@
	# Verificar imagen Multiboot v1.
	grub-file --is-x86-multiboot $@

%.o: %.S
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f kernel *.o core

.PHONY: clean