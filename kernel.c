#include "idt/idt.h"
#include "io/term.h"
#include "serial/serial.h"
#include "vga/vga.h"

vga_state_t kernel_vga;
void kmain() {
  kernel_vga = k_vga_init((vga_t *)0xB8000);
  k_vga_clear(&kernel_vga);

  serial_init();
  k_vga_print("Serial online!\n", &kernel_vga);
  serial_print("Serial online!\n");

  idt_init();
  k_vga_print("IDT Loaded!\n", &kernel_vga);
  serial_print("IDT Loaded!\n");

  kprintf("Formating active.\n");
}
