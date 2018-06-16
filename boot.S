

#include "multiboot.h"

#define KSTACK_SIZE 8192

.align 4
multiboot:
    .long MULTIBOOT_HEADER_MAGIC
    .long 0
    .long -(MULTIBOOT_HEADER_MAGIC)

.globl _start
_start:
    // Paso 1: Configurar el stack antes de llamar a kmain.
    movl $0, %ebp
    movl $(kstack + KSTACK_SIZE), %esp
    push %ebp

    // Paso 2: pasar la información multiboot a kmain. Si el
    // kernel no arrancó vía Multiboot, se debe pasar NULL.
    //
    // Usar una instrucción de comparación (TEST o CMP) para
    // comparar con MULTIBOOT_BOOTLOADER_MAGIC, pero no usar
    // un salto a continuación, sino una instrucción CMOVcc
    // (copia condicional).
    // ...
    mov $0, %ecx
    CMP %eax, MULTIBOOT_BOOTLOADER_MAGIC

    //CMV mueve memoria si la condicion anterior es verddera.
    //al llamar a una funcion el kernel debe guardar los parametros en el stack asignado
    //ebp apunta a mi stack actual, no? que sería la informacion de multiboot? es lo que esta en el registro %ebx? 

    CMOVE %ecx, %ebx
    push %ebx
    call kmain
halt:
    hlt
    jmp halt

.data
.p2align 3
kstack:
    .space KSTACK_SIZE
