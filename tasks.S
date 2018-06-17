// Realiza una llamada a "entry" sobre el stack proporcionado.
//extern void task_exec(uintptr_t entry, uintptr_t stack);

.text

.globl task_exec

task_exec:
  // Preámbulo estándar
  push %ebp
  movl %esp, %ebp

  //obtener parametros del stack
  movl 8(%ebp), %eax
  movl 12(%ebp), %esp
  //llamado a funcion pasada por stack
  call %eax

  //return
  movl %ebp, %esp
  pop  %ebp
  ret
