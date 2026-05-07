#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_desc {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idt_desc_t;

void idt_init();

#endif
