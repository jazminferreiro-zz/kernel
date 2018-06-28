#include "decls.h"
#include <stdbool.h>

#define SHIFT_CODE 0x2a

/**
 * Handler para el timer (IRQ0). Escribe un carácter cada segundo.
 */
static const uint8_t hz_ratio = 18;  // Default IRQ0 freq (18.222 Hz).

void timer() {
    static char chars[81];
    static unsigned ticks;
    static int8_t line = 20;
    static uint8_t idx = 0;

    if (++ticks % hz_ratio == 0) {
        chars[idx] = '.';
        chars[++idx] = '\0';
        vga_write(chars, line, 0x07);
    }

    if (idx >= sizeof(chars)) {
        line++;
        idx = 0;
    }
}

/**
 * Mapa de "scancodes" a caracteres ASCII en un teclado QWERTY.
 */

static char klayout[128] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8',             // 0-9
    '9', '0', 0,   0,   0,   0,   'q', 'w', 'e', 'r',             // 10-19
    't', 'y', 'u', 'i', 'o', 'p', 0,   0,   0,   0,               // 20-29
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0,   0,          // 30-40
    0,   SHIFT_CODE,   /*left shift*/
    0,   'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.'};  // 41-52

static char mayus_klayout[128] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8',             // 0-9
    '9', '0', 0,   0,   0,   0,   'Q', 'W', 'E', 'R',             // 10-19
    'T', 'Y', 'U', 'I', 'O', 'P', 0,   0,   0,   0,               // 20-29
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0,   0,          // 30-40
    0,   SHIFT_CODE,   /*left shift*/
    0,   'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.'};  // 41-52
static const uint8_t KBD_PORT = 0x60;
static uint8_t KEYSTATUS = 0;
/**
 * Handler para el teclado (IRQ1).
 *
 * Imprime la letra correspondiente por pantalla.
 */
void keyboard() {
  uint8_t code;
  static char chars[81];
  static uint8_t idx = 0;

  asm volatile("inb %1,%0" : "=a"(code) : "n"(KBD_PORT));


  if (code & 0x80) {
    //se solto tecla
    code = code & 0x7F; 
    // tabla de 52 posiciones: 6 bits
    if (code >= sizeof(klayout) || !klayout[code])
      return;
    if (klayout[code] == SHIFT_CODE)
      KEYSTATUS = 0x0;
  } else {
    if (code >= sizeof(klayout) || !klayout[code])
      return;

    if (idx == 80) {
      while (idx--)
        chars[idx] = ' ';
    }

    if (klayout[code] == SHIFT_CODE) {
      KEYSTATUS = 0x01;
      return;
    }

    if (KEYSTATUS == 0x01)
      chars[idx++] = mayus_klayout[code];
    else
      chars[idx++] = klayout[code];
    vga_write(chars, 19, 0x0A);
  }
}
