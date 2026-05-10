#include <stdint.h>
#include "../../helpers/helper.h"

#define PIT_FREQ 1193182

void pit_init(uint32_t freq) {
    uint32_t divisor = PIT_FREQ / freq;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}
