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

  Tasks[i].status = READY;
  Tasks[i].frame->edi = 0;
  Tasks[i].frame->esi = 0;
  Tasks[i].frame->ebp = 0;
  Tasks[i].frame->esp = 0;
  Tasks[i].frame->ebx = 0;
  Tasks[i].frame->edx = 0;
  Tasks[i].frame->ecx = 0;
  Tasks[i].frame->eax = 0;
  Tasks[i].frame->eip;
  Tasks[i].frame->cs;
  Tasks[i].frame->padding =0;
  Tasks[i].frame->eflags = IF;
}

void sched(struct TaskFrame *tf) {
    // ...
}
