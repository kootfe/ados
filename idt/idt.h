#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_init();

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

typedef struct __attribute__((packed)) {
  uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
  uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;

  uint64_t vector;
  uint64_t err;

  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
} isr_frame_t;

extern idt_entry_t idt[256];

#endif
