#include "decls.h"

volatile char *const VGABUF = (volatile char *) 0xb8000;

void vga_write(const char *s, int8_t linea, uint8_t color){
	//*(VGABUF + 120) += 88;
    volatile char *buf = VGABUF;
    buf += 2 * 80 * linea;

    while( *s != 0 ){
        *buf++ = *s++;
        *buf++ = color;
    }
}

void __attribute__((regparm(2))) vga_write_cyan(const char *s, int8_t linea) {
    vga_write(s, linea, 0xB0);
}

static size_t int_width(uint64_t val) {
    uint64_t contador= 0;
    while (val > 0){
		contador++;
		val /= 10;
	}
	return contador;
}




bool fmt_int(uint64_t val, char *s, size_t bufsize){
	
	if( int_width(val) >= bufsize){
		return false;
	}
	

	for (int i =  int_width(val) - 1; i >= 0; i--){
		*(s++) = val % 10 + '0';
		val /= 10;
	}
	*(s++) = '\0';
	return true;
}