#include "decls.h"
#include "multiboot.h"
#include "stacks.S"

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);

    // A remplazar por una llamada a two_stacks(),
    // definida en stacks.S.
    //vga_write("vga_write() from stack1", 12, 0x17);
    //vga_write("vga_write() from stack2", 13, 0x90);
    two_stacks();
}
