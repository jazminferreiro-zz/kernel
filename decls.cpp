#include "decls.h"

static volatile char *const VGABUF = (volatile char *) 0xb8000;

void vga_write(const char *s, int8_t linea, uint8_t color){
	*(VGABUF + 120) += 88;

    volatile char *buf = VGABUF;

    while( *s != 0 ){
        *buf++ = *s++;
        *buf++ = color;
    }
}