#ifndef VGA_H
#define VGA_H

#include <stdint.h>

typedef volatile uint16_t vga_t;

#define COLS 80
#define ROWS 25
#define VGA_SIZE (COLS * ROWS)
// Lets just use defults for now

typedef struct vga_state {
  int row;
  int col;
  vga_t *buff;
} vga_state_t;

void k_vga_putc(const char c, int code, int idx, vga_t *buff);
void k_vga_print(const char *str, vga_state_t *state);
vga_state_t k_vga_init(vga_t *buff);

#endif
