#ifndef KERN2_DECL_H
#define KERN2_DECL_H

#include <stdint.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

void __attribute__((regparm(3)))
vga_write2(const char *s, int8_t linea, uint8_t color);
void __attribute__((regparm(2))) vga_write_cyan(const char *s, int8_t linea);
void vga_write(const char *s, int8_t linea, uint8_t color);


static size_t int_width(uint64_t val);

bool fmt_int(uint64_t val, char *s, size_t bufsize);


extern void task_exec(uintptr_t entry, uintptr_t stack);
extern void task_swap(uintptr_t *esp);
extern void two_stacks();
void two_stacks_c();
void contador_run();

extern void breakpoint();
extern void divzero();
void irq_init();
void timer();
extern void ack_irq();
extern void timer_asm();

#endif
