#include "helpers/helper.h"
#include "idt/idt.h"
#include "io/pic.h"
#include "io/term.h"
#include "serial/serial.h"
#include "shell/shell.h"
#include "timer/pit/pit.h"
#include "vga/vga.h"
#include <stdint.h>

int PIT_FREQ = 100;

vga_state_t kernel_vga;
void kmain()
{
    kernel_vga = k_vga_init((vga_t *)0xB8000);
    k_vga_clear(&kernel_vga);

    serial_init();
    k_vga_print("Serial online!\n", &kernel_vga);
    serial_print("Serial online!\n");

    kprintf("Formating active.\n");

    pic_remap();
    kprintf("PIC Remaped.\n");

    enable_keyboard();
    kprintf("Keyboard Enabled.\n");

    idt_init();
    kprintf("IDT Loaded!\n");
    kprintf("IDT[2] offset: %llx\n", ((uint64_t)idt[2].offset_high << 32) |
                                         ((uint64_t)idt[2].offset_mid << 16) | idt[2].offset_low);
    outb(0x70, inb(0x70) | 0x80);

    pit_init(PIT_FREQ);
    kprintf("PIT Started, freq: %u\n", PIT_FREQ);
    shell_prompt();

    __asm__ volatile("sti");
    for (;;)
        __asm__ volatile("hlt");
}
