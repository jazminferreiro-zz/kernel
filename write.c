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
    int divisor = 10;
    size_t count = 0;
    uint64_t r = 0;
    while(r < val){
        r = val % divisor;
        divisor = divisor * 10;
        count++;
    }
    return count;
}


// Escribe en ‘s’ el valor de ‘val’ en base 10 si su anchura
// es menor que ‘bufsize’. En ese caso devuelve true, caso de
// no haber espacio suficiente no hace nada y devuelve false.
bool fmt_int(uint64_t val, char *s, size_t bufsize){
	if(int_with(val) >= bufsize){
		return false;
	}

	int decimal_num = 0;
	int i = 0;
	int resto;
	while (val!=0){
        resto = val%10;
        val /= 10;
        decimal_num += resto*pow(2,i);
        ++i;
    }

    s += 1;
    *(s--) = '\0';
    int divisor = 1;
    resto = 0;

    while((decimal_num / (divisor*10)) > 0){
    	*(s--) = (char)((decimal_num% (divisor*10)) - resto)/divisor;
    	resto = decimal_num % divisor*10;
    	divisor = divisor*10;
    	i--;
    }

	return true;
}
