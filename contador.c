#include "decls.h"

#define COUNTLEN 20
#define TICKS (1ULL << 15)
#define DELAY(x) (TICKS << (x))
#define USTACK_SIZE 4096

static volatile char *const VGABUF = (volatile void *) 0xb8000;

static uintptr_t esp;
static uint8_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
static uint8_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));

extern void task_exec(uintptr_t entry, uintptr_t stack);
extern void task_swap(uintptr_t *esp);

static void yield() {
    if (esp)
        task_swap(&esp);
}

static void contador_yield(unsigned lim, uint8_t linea, char color) {
    char counter[COUNTLEN] = {'0'};  // ASCII digit counter (RTL).

    while (lim--) {
        char *c = &counter[COUNTLEN];
        volatile char *buf = VGABUF + 160 * linea + 2 * (80 - COUNTLEN);

        unsigned p = 0;
        unsigned long long i = 0;

        while (i++ < DELAY(6))  // Usar un entero menor si va demasiado lento.
            ;

        while (counter[p] == '9') {
            counter[p++] = '0';
        }

        if (!counter[p]++) {
            counter[p] = '1';
        }

        while (c-- > counter) {
            *buf++ = *c;
            *buf++ = color;
        }

        yield();
    }
}

/*    la configuración del stack del primer contador, que se ejecuta con task_exec(), será muy similar a las realizadas en la función two_stacks_c() del ejercicio kern2-exec.

    la configuración del segundo contador es más compleja, y seguramente sea mejor realizarla tras implementar task_swap(); pues se debe crear artificialmente el stack tal y como lo hubiera preparado esta función.*/

void contador_run() {
    // Configurar stack1 y stack2 con los valores apropiados.
    uintptr_t *a = (uintptr_t *) &stack1[USTACK_SIZE-1];
    uintptr_t *b = (uintptr_t *) &stack2[USTACK_SIZE-1];

    //contador_yield(100, 0, 0x2F);
    *(a--) = (uintptr_t) 0x2F;
    *(a--) = (uintptr_t) 0;
    *(a) = (uintptr_t) 100;

    //contador_yield(100, 1, 0x4F);
    *(b--) = (uintptr_t) 0x4F;
    *(b--) = (uintptr_t) 1;
    *(b--) = (uintptr_t) 100;

    *(b--) = 0; //ebx
    *(b--) = 0; //esi
    *(b--) = 0; //edi
    *(b--) = 0; //ebx

    *(b--) = 0; // para retorno de task_swap
    *(b) = (uintptr_t) contador_yield; // cual deberia ser la dir de retorno??

    // Actualizar la variable estática ‘esp’ para que apunte
    // al del segundo contador.
    esp = (uintptr_t) b;

    // Lanzar el primer contador con task_exec.
    task_exec((uintptr_t) contador_yield, (uintptr_t) a);
}
