#include "decls.h"
#include "interrupts.h"

static struct IDTR idtr;
static struct Gate idt[255]; // mas chico?? 34

// Multiboot siempre define "8" como el segmento de código.
// (Ver campo CS en `info registers` de QEMU.)
static const uint8_t KSEG_CODE = 8;

// Identificador de "Interrupt gate de 32 bits" (ver IA32-3A,
// tabla 6-2: IDT Gate Descriptors).
static const uint8_t STS_IG32 = 0xE;

void idt_install(uint8_t n, void (*handler)(void)) {
    uintptr_t addr = (uintptr_t) handler;

    idt[n].rpl = 0;
    idt[n].type = STS_IG32;
    idt[n].segment = KSEG_CODE;

    idt[n].off_15_0 = addr & 0xFF;
    idt[n].off_31_16 = addr >> 16;

    idt[n].present = 1;
}

extern void breakpoint();
extern void divzero();

void idt_init() {
  // (1) Instalar manejadores ("interrupt service routines").
  vga_write("idt loading.............", 17, 0x70);
  //excepciones
  idt_install(T_BRKPT, breakpoint);
  //idt_install(T_DIVIDE, divzero);

  // (2) Configurar ubicación de la IDT.
  idtr.base = (uintptr_t) idt;
  idtr.limit = 255*8 - 1;

  // (3) Activar IDT.
  asm("lidt %0" : : "m"(idtr));
}

///////////////////////////////////////////////////
////////////IRQ////////////////////////////////////
///////////////////////////////////////////////////
#define outb(port, data) asm("outb %b0,%w1" : : "a"(data), "d"(port));

extern void ack_irq();

static void irq_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void irq_init() {
  // (1) Redefinir códigos para IRQs.
  irq_remap();

  // (2) Instalar manejadores.
  //idt_install(T_TIMER, ack_irq);
  //idt_install(T_KEYBOARD, ack_irq);

  // (3) Habilitar interrupciones.
  asm("sti");
}
