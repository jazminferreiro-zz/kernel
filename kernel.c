static volatile char *const VGABUF = (volatile void *) 0xb8000;

#include "decls.h"
#include "multiboot.h"	

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);
}