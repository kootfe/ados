#include "vga.h"
#include <stdint.h>
#include "../helpers/helper.h"

static void vga_scroll(vga_state_t *state) {
  if (state->row < ROWS)
    return;
  for (int i = 0; i < (ROWS - 1) * COLS; ++i) {
    state->buff[i] = state->buff[i + COLS];
  }

  for (int i = (ROWS - 1) * COLS; i < ROWS * COLS; i++) {
    state->buff[i] = ' ' | (0x07 << 8);
  }

  state->row = ROWS - 1;
}

static void vga_update_cursor(int row, int col) {
    uint16_t pos = row * COLS + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));

}

void k_vga_putc(const char c, int code, int idx, vga_t *buff) {
  buff[idx] = c | (code << 8);
}

void k_vga_print(const char *str, vga_state_t *state) {
  for (int i = 0; str[i]; ++i) {
    int idx = state->row * COLS + state->col;
    if (str[i] == '\n') {
      state->col = 0;
      state->row++;
      vga_scroll(state);
      continue;
    } else
      k_vga_putc(str[i], 0x07, idx, state->buff);
    state->col++;
    if (state->col >= COLS) {
      state->col = 0;
      state->row++;
    }

    vga_scroll(state);
    vga_update_cursor(state->row, state->col);
  }
}

vga_state_t k_vga_init(vga_t *buff)
{
  vga_state_t tmp = {.row = 0, .col = 0, .buff = buff};
  return tmp;
}

void k_vga_clear(vga_state_t *state)
{
    for (int i = 0; i < VGA_SIZE; ++i)
        state->buff[i] = ' ' | (0x07 << 8);
    state->row = 0;
    state->col = 0;
    vga_update_cursor(0, 0);
}
