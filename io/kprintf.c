#include "../serial/serial.h"
#include "../vga/vga.h"
#include "term.h"
#include <stdarg.h>

extern vga_state_t kernel_vga;

static void kprint_str(const char *s) {
  k_vga_print(s, &kernel_vga);
  serial_print(s);
}

static void kprint_char(char c) {
  char buff[2] = {c, 0};
  kprint_str(buff);
}

static void kprint_uint(unsigned long long n, int base) {
  char digits[] = "0123456789abcdef";
  char buff[64];
  int i = 63;
  buff[i] = 0;

  if (n == 0) {
    kprint_char('0');
    return;
  }

  while (n && i > 0) {
    buff[--i] = digits[n % base];
    n /= base;
  }

  kprint_str(&buff[i]);
}

static void kprint_int(long long n) {
  if (n < 0) {
    kprint_char('-');
    kprint_uint((unsigned long long)-n, 10);
  } else
    kprint_uint((unsigned long long)n, 10);
}

void kprintf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  for (int i = 0; fmt[i]; ++i) {
    if (fmt[i] != '%') {
      kprint_char(fmt[i]);
      continue;
    }

    ++i;
    switch (fmt[i]) {
    case 'd':
      kprint_int(va_arg(args, int));
      break;
    case 'u':
      kprint_uint(va_arg(args, unsigned int), 10);
      break;
    case 'x':
      kprint_uint(va_arg(args, unsigned int), 16);
      break;
    case 'p':
      kprint_uint(va_arg(args, unsigned long long), 16);
      break;
    case 's':
      kprint_str(va_arg(args, const char *));
      break;
    case 'c':
      kprint_char(va_arg(args, int));
      break;
    case 'l':
      ++i;
      if (fmt[i] == 'l') {
        ++i;
      }
      switch (fmt[i]) {
      case 'u':
        kprint_uint(va_arg(args, unsigned long long), 10);
        break;
      case 'x':
        kprint_uint(va_arg(args, unsigned long long), 16);
        break;
      }
      break;
    case '%':
      kprint_char('%');
      break;
    }
  }

  va_end(args);
}
