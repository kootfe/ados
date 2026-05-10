#include "idt.h"

idt_entry_t idt[256];
idt_desc_t idt_desc;

extern void *isr_stub_table[];

static void idt_set_gate(int n, void *handler) {
    uint64_t addr = (uint64_t)handler;

    idt[n].offset_low = addr & 0xFFFF;
    idt[n].selector = 0x08;
    idt[n].ist = 0;
    idt[n].type_attr = 0x8E;
    idt[n].offset_mid = (addr >> 16) & 0xFFFF;
    idt[n].offset_high = (addr >> 32) & 0xFFFFFFFF;
    idt[n].zero = 0;
}

extern void lidt(void*);

void idt_init() {
    for (int i = 0; i < 49; ++i)
        idt_set_gate(i, isr_stub_table[i]);

    idt_desc.limit = sizeof(idt) - 1;
    idt_desc.base = (uint64_t)&idt;

    lidt(&idt_desc);
}
