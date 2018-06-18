#ifndef KERN2_DECL_H
#define KERN2_DECL_H

#include <stdint.h>

void __attribute__((regparm(3)))
vga_write2(const char *s, int8_t linea, uint8_t color);

void vga_write(const char *s, int8_t linea, uint8_t color);

void contador_run();

#endif
