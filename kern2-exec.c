#include "decls.h"
#include "multiboot.h"

#define USTACK_SIZE 4096

extern void task_exec(uintptr_t entry, uintptr_t stack);

void two_stacks_c();

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);

    //two_stacks();
    two_stacks_c();
}

static uint8_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
static uint8_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));

void two_stacks_c() {
    // Inicializar al *tope* de cada pila.
    uintptr_t *a = (uintptr_t *) &stack1[USTACK_SIZE];
    uintptr_t *b = &stack2[USTACK_SIZE];

    // Preparar, en stack1, la llamada:
    // vga_write("vga_write() from stack1", 15, 0x57);

    // AYUDA 1: se puede usar alguna forma de pre- o post-
    // incremento/decremento, según corresponda:
    //
    //     *(a++) = ...
    //     *(++a) = ...
    //     *(a--) = ...
    //     *(--a) = ...
    *(a--) = (uintptr_t) "vga_write() from stack1";
    *(a--) = (uintptr_t) 15;
    *(a--) = (uintptr_t) 0x57;
    // AYUDA 2: para apuntar a la cadena con el mensaje,
    // es suficiente con el siguiente cast:
    //
    //   ... a ... = (uintptr_t) "vga_write() from stack1";

    // Preparar, en s2, la llamada:
    // vga_write("vga_write() from stack2", 16, 0xD0);

    // AYUDA 3: para esta segunda llamada, usar esta forma de
    // asignación alternativa:
    b -= 3;
    b[0] = (uintptr_t) "vga_write() from stack2";
    b[1] = 16;
    b[2] = 0xD0;

    // Primera llamada usando task_exec().
    task_exec((uintptr_t) vga_write, (uintptr_t) &stack1[USTACK_SIZE]);

    // Segunda llamada con ASM directo. Importante: no
    // olvidar restaurar el valor de %esp al terminar, y
    // compilar con: -fasm -fno-omit-frame-pointer.
    //asm("...; call *%1; ..."
      //  : /* no outputs */
        //: "r"(stack2), "r"(vga_write));
}
