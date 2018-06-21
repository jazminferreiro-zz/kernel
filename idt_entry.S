
//.globl breakpoint
//breakpoint:
        // Manejador mínimo.
//        iret
#define PIC1 0x20
#define ACK_IRQ 0x20


.globl breakpoint
breakpoint:
        // Preámbulo estándar
        //push %ebp
        //movl %esp, %ebp

        // (1) Guardar registros.
        push %ebx
        push %edi
        push %esi
        push %ebp

        // (2) Preparar argumentos de la llamada.
        movl $0xB0, %eax
        push %eax
        movl $14, %eax
        push %eax
        movl $breakpoint_msg, %eax
        push %eax

        // (3) Invocar a vga_write2()
        // vga_write2("Hello, breakpoint", 14, 0xB0);
        call vga_write2

        // (4) Restaurar registros.
        pop %ebp
        pop %esi
        pop %edi
        pop %ebx

        // (5) Finalizar ejecución del manejador.
        iret
.data
breakpoint_msg:
        .asciz "Hello, breakpoint"
