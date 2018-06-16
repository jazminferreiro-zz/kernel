
#include "decls.h"
#include "multiboot.h"
#include <string.h>	

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);

    if (mbi->flags) {
        char buf[256] = "cmdline: ";
        char *cmdline = (void *) mbi->cmdline;
        // Aquí usar strlcat() para concatenar cmdline a buf.
        strncat(buf, cmdline,  sizeof(buf) - strlen(buf) - 1);
        vga_write(buf, 9, 0x07);

        // A remplazar por una llamada a two_stacks(),
    	// definida en stacks.S.
        extern two_stacks();
        two_stacks();
    	vga_write("vga_write() from stack1", 12, 0x17);
    	vga_write("vga_write() from stack2", 13, 0x90);
    }
}	