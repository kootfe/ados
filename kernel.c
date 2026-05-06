#include "vga/vga.h"

void kmain() {
  vga_state_t state = k_vga_init((vga_t *)0xB8000);
  k_vga_print("Merhaba, Ado!", &state);
}
