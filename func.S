// func.S


.align 4

.text

.globl vga_write2

vga_write2:
        push %ebp
        movl %esp, %ebp

        // color esta en ecx
        push %ecx
        // linea esta en edx
        // 2*80 = 2*2*2*2*2 * 5
        // sal $5, %edx
        push %edx
        // *s esta en eax???
        movl 16(%ebp), %eax
        push %eax

        call vga_write

        leave
        ret
