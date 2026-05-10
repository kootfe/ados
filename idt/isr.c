#include "../io/keyboard.h"
#include "../io/pic.h"
#include "../io/term.h"
#include "idt.h"
#include <stdint.h>

volatile uint64_t ticks = 0;

static const char *excv_name[] = {
    "Divide by Zero",       // 0
    "Debug",                // 1
    "NMI",                  // 2
    "Breakpoint",           // 3
    "Overflow",             // 4
    "Bound Range Exceeded", // 5
    "Invalid Opcode",       // 6
    "Device Not Available", // 7
    "Double Fault",         // 8
    "Coprocessor Overrun",  // 9
    "Invalid TSS",          // 10
    "Segment Not Present",  // 11
    "Stack Fault",          // 12
    "General Protection",   // 13
    "Page Fault",           // 14
    "Reserved",             // 15
    "x87 FPU Error",        // 16
    "Alignment Check",      // 17
    "Machine Check",        // 18
    "SIMD FPU Error",       // 19
    "Virtualization",       // 20
    "Control Protection",   // 21
    "Reserved",             // 22
    "Reserved",             // 23
    "Reserved",             // 24
    "Reserved",             // 25
    "Reserved",             // 26
    "Reserved",             // 27
    "Reserved",             // 28
    "Hypervisor Injection", // 29
    "VMM Communication",    // 30
    "Security Exception",   // 31
};

void isr_handler(isr_frame_t *frame)
{
    if (frame->vector > 49)
    {
        kprintf("BAD FRAME: vector=%u err=%u\n", frame->vector, frame->err);
        for (;;)
            __asm__("cli;hlt");
    }

    int n = (int)frame->vector;
    uint64_t err = frame->err;
    if (n >= 32 && n <= 47)
    {
        if (n == 32)
        {
            ticks++;
        }
        else if (n == 33)
        {
            keyboard_handler();
        }

        pic_eoi(n - 32);
        return;
    }
    kprintf("\n--- EXCEPTION ---\n");
    kprintf("Name: %s\n", n < 32 ? excv_name[n] : "Unknown");
    kprintf("Vector: %d\n", n);
    kprintf("Error: %x\n", err);

    if (n == 14)
    {
        uint64_t cr2;
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        kprintf("CR2: %p\n", cr2);
        kprintf("Reason: %s%s%s%s%s\n", err & 0x1 ? "Protection" : "not-present",
                err & 0x2 ? " write" : " read", err & 0x4 ? " user" : " kernel",
                err & 0x8 ? " rsvd-bit" : "", err & 0x10 ? " fetch" : "");
    }
    kprintf("---------------------\n");
    for (;;)
        __asm__ volatile("cli; hlt");
}
