#include "decls.h"
#include "sched.h"

#define MAX_TASK 10
#define IF 0x100

static struct Task Tasks[MAX_TASK];
static struct Task *current;

void sched_init() {
  //Esta función debe inicializar la variable global current
  //al primer elemento del arreglo global de tareas, y poner su estado en RUNNING.
  current = &Tasks[0];
  current->status = RUNNING;
}

void spawn(void (*entry)(void)) {
  //La función spawn() debe:
  //Encontrarn el el arreglo global Tasks, una entrada con estado FREE
  //Cambiar su status a READY
  //Inicializar todos sus registros a cero,
  //excepto %cs, %eip y %eflags.
  // En particular %eflags debe contener el flag IF,
  // o las interrupciones de reloj no se habilitarán al entrar la tarea en ejecución.

  int i = 0;
  for(i = 0; i < MAX_TASK; i++){
    if(Tasks[i].status == FREE){
      break;
    }
  }

uint8_t* stack = &Tasks[i].stack[4096];
Tasks[i].frame = (struct TaskFrame *) ((--stack) - sizeof(struct TaskFrame));
  Tasks[i].status = READY;
  //Tasks[i].frame = (struct TaskFrame *) &Tasks[i].stack[4096 - sizeof(struct TaskFrame) ];
  Tasks[i].frame->edi = 0;
  Tasks[i].frame->esi = 0;
  Tasks[i].frame->ebp = 0;
  Tasks[i].frame->esp = 0;
  Tasks[i].frame->ebx = 0;
  Tasks[i].frame->edx = 0;
  Tasks[i].frame->ecx = 0;
  Tasks[i].frame->eax = 0;

  Tasks[i].frame->eip = (uint32_t) entry;
  Tasks[i].frame->cs = 8;//(uint32_t) entry;
  Tasks[i].frame->padding = 0;
  Tasks[i].frame->eflags = IF;
}

/*
encontrar, de manera round-robin, la siguiente tarea que se encuentra en estado
READY.
*/
void sched(struct TaskFrame *tf) {
  //Una posible manera es encontrar en el arreglo la tarea en ejecución, y
  //buscar a partir de ahí:
  struct Task *new = 0;
  struct Task *old = current;
  int i = 0;
  for(i = 0; i < MAX_TASK; i++){
    if(Tasks[i].status == RUNNING){
      break;
    }
  }
  i = i + 1;
  new = &Tasks[i];
  while (new != old) {
    if (i == MAX_TASK)
      i = 0;
    if (new->status == READY)
      break;
    new = &Tasks[i++];//?
  }

  //si se la encuentra, se debe poner old->status a READY y
  //guardar en old->frame el frame recibido como parámetro;
  old->status = READY;
  old->frame = tf;
  //actualizar la variable global current y en current->status poner RUNNING.
  current = new;
  current->status = RUNNING;

  //Por último, para ejecutar el cambio, se puede usar el siguiente assembler:
  asm("movl %0, %%esp\n"
    "popa\n"
    "iret\n"
    :
    : "g"(current->frame)
    : "memory");

}
