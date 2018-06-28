
#define PIC1 0x20
#define ACK_IRQ 0x20


.globl breakpoint
breakpoint:

        // (1) Guardar registros.
        push %eax
        push %edx
        push %ecx
        // Preámbulo estándar
        push %ebp
        movl %esp, %ebp

        // (2) Preparar argumentos de la llamada.
        movl $0xB0, %ecx
        movl $14, %edx
        movl $breakpoint_msg, %eax

        // (3) Invocar a vga_write2()
        // vga_write2("Hello, breakpoint", 14, 0xB0);
        call vga_write2

        // (4) Restaurar registros.
        leave
        pop %ecx
        pop %edx
        pop %eax

        // (5) Finalizar ejecución del manejador.
        iret


.globl ack_irq
ack_irq:
        // Indicar que se manejó la interrupción.
        movl $ACK_IRQ, %eax
        outb %al, $PIC1
        iret


.globl timer_asm
timer_asm:
        // Guardar registros.
        push %eax
        push %ecx
        push %edx
        // hay que guardar?
        call timer
        // Restaurar registros.
        pop %edx
        pop %ecx
        pop %eax

        jmp ack_irq


.globl divzero
divzero:
        // Guardar registros
        push %eax
        push %ecx
        push %edx
        // incrementar ebx para cuando se reintente
        inc %ebx

        // (3) Invocar a vga_write_cyan()
        // vga_write_cyan("Se divide por ++ebx", 17);
        movl $17, %edx
        movl $divzero_msg, %eax
        call vga_write_cyan

        // Restaurar registros
        pop %edx
        pop %ecx
        pop %eax
        iret

.data
divzero_msg:
        .asciz "Se divide por ++ebx"
breakpoint_msg:
        .asciz "Hello, breakpoint"
