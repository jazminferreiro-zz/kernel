#include "decls.h"
#include "multiboot.h"
#include "interrupts.h"

#define USTACK_SIZE 4096

void kmain(const multiboot_info_t *mbi) {
    int8_t linea;
    uint8_t color;

    vga_write("kern2 loading.............", 8, 0x70);

    two_stacks();
    two_stacks_c();
    contador_run();

    idt_init();
    asm("int3");
    irq_init();

    asm("div %4"
        : "=a"(linea), "=c"(color)
        : "0"(18), "1"(0xE0), "b"(0), "d"(0));

    vga_write2("Funciona vga_write2?", 18, 0xE0);
}


static uint8_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
static uint8_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));

void two_stacks_c() {
    // Inicializar al *tope* de cada pila.
    uintptr_t *a = (uintptr_t *) &stack1[USTACK_SIZE-1];
    uintptr_t *b = (uintptr_t *) &stack2[USTACK_SIZE];

    // Preparar, en stack1, la llamada:
    // vga_write("vga_write() from stack1", 15, 0x57);
    *(a--) = (uintptr_t) 0x57;
    *(a--) = (uintptr_t) 15;
    *(a) = (uintptr_t) "vga_write() from stack1";

    // Preparar, en s2, la llamada:
    // vga_write("vga_write() from stack2", 16, 0xD0);
    b -= 3;
    b[0] = (uintptr_t) "vga_write() from stack2";
    b[1] = 16;
    b[2] = 0xD0;

    // Primera llamada usando task_exec().
    task_exec((uintptr_t) vga_write, (uintptr_t) a);

    // Segunda llamada con ASM directo.
    asm("push %%ebp; movl %%esp, %%ebp; movl %0, %%esp; call *%1; leave;"
        : /* no outputs */
        : "r"(b), "r"(vga_write));
}
