#include "vga.h"

void k_vga_putc(const char c, int code, int idx, vga_t *buff) {
  buff[idx] = c | (code << 8);
}

void k_vga_print(const char *str, vga_state_t *state) {
  for (int i = 0; str[i]; ++i) {
    int idx = state->row * COLS + state->col;
    if (str[i] == '\n') {
      state->col = 0;
      state->row++;
      if (state->row >= ROWS)
        state->row = 0;
      continue;
    } else
      k_vga_putc(str[i], 0x07, idx, state->buff);
    state->col++;
    if (state->col >= COLS) {
      state->col = 0;
      state->row++;
    }

    if (state->row >= ROWS) {
      state->row = 0;
    }
  }
}

vga_state_t k_vga_init(vga_t *buff) {
  vga_state_t tmp = {.row = 0, .col = 0, .buff = buff};
  return tmp;
}
