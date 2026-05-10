#include "serial.h"
#include <stdint.h>
#include "../helpers/helper.h"

void serial_init()
{
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1 + 0, 0x03);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);
}

static int serial_tx_empty() {
    return inb(COM1 + 5) & 0x20;
}

void serial_putc(char c) {
    while (!serial_tx_empty());
    outb(COM1, c);
}

void serial_print(const char *str) {
    for (int i = 0; str[i]; ++i) {
        if (str[i] == '\n')
            serial_putc('\r');
        serial_putc(str[i]);
    }
}
