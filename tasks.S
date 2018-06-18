// Realiza una llamada a "entry" sobre el stack proporcionado.
//extern void task_exec(uintptr_t entry, uintptr_t stack);

.align 4

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

/////////////////////////////////////////////////////////////
// Pone en ejecución la tarea cuyo stack está en ‘*esp’, cuyo
// valor se intercambia por el valor actual de %esp. Guarda y
// restaura todos los callee-saved registers.
//
// void task_swap(uintptr_t *esp);

.globl task_swap
task_swap:

//    guardar, en el stack de la tarea actual, los registros que son callee-saved: ebx, edi, esi
  push %ebx
  push %edi
  push %esi
  push %ebp

// cargar en %esp el stack de la nueva tarea, y guardar en la variable esp el valor previo de %esp
  movl 20(%esp), %eax
  movl %esp, %edx
  movl (%eax), %esp
  movl %edx, (%eax) //guardo stack anterior en este stack

// restaurar, desde el nuevo stack, los registros que fueron guardados por una llamada previa a task_swap(), y retornar (con la instrucción ret) a la nueva tarea.
  pop %ebp
  pop %esi
  pop %edi
  pop %ebx

  ret
