#include "shell.h"
#include "../io/term.h"
#include "../vga/vga.h"
#include <stddef.h>
#include <stdint.h>

#define BUFF_SIZE 256

extern volatile uint64_t ticks;
extern vga_state_t kernel_vga;

static char buff[BUFF_SIZE];
static int buff_pos = 0;

static int strcmp(const char *a, const char *b)
{
    while (*a && *b && *a == *b)
    {
        a++;
        b++;
    }
    return *a - *b;
}

static void shell_exec(const char *cmd)
{
    if (strcmp(cmd, "help") == 0)
    {
        kprintf("Commands:\nhelp, clear, version, ticks, cr\n");
    }
    else if (strcmp(cmd, "version") == 0)
    {
        kprintf("Ados - v0.1\n");
    }
    else if (strcmp(cmd, "clear") == 0)
    {
        k_vga_clear(&kernel_vga);
    }
    else if (strcmp(cmd, "ticks") == 0)
    {
        kprintf("ticks: %llu\n", ticks);
    }
    else if (strcmp(cmd, "cr") == 0)
    {
        uint64_t cr0, cr2, cr3, cr4;
        __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
        __asm__ volatile("mov %%cr4, %0" : "=r"(cr4));
        kprintf("CR0: %llx\nCR2: %llx\nCR3: %llx\nCR4: %llx\n", cr0,cr2,cr3,cr4);
    }
    else if (cmd[0] != '\0')
    {
        kprintf("unknown: %s\n", cmd);
    }
}

void shell_prompt() { kprintf("> "); }

void shell_feed(char c)
{
    if (c == '\n') {
        kprintf("\n");
        buff[buff_pos] = '\0';
        shell_exec(buff);
        buff_pos = 0;
        shell_prompt();
    }
    else if (c == '\b') {
        return;
 /*       if (buff_pos > 0) {
            buf_pos
        }*/
    } else {
        if (buff_pos < BUFF_SIZE - 1) {
            buff[buff_pos++] = c;
            kprintf("%c", c);
        }
    }
}
