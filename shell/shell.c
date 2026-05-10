#include "shell.h"
#include "../helpers/helper.h"
#include "../io/term.h"
#include "../vga/vga.h"
#include <stddef.h>
#include <stdint.h>

#define BUFF_SIZE 256
extern int PIT_FREQ;
extern volatile uint64_t ticks;
extern vga_state_t kernel_vga;

static char buff[BUFF_SIZE];
static int buff_pos = 0;

static uint8_t cmos_read(uint8_t reg)
{
    outb(0x70, reg);
    return inb(0x71);
}

static void second_to_ddmmss(int total_secs, int *dd, int *mm, int *ss)
{
    *dd = total_secs / 3600;
    *mm = (total_secs % 3600) / 60;
    *ss = total_secs % 60;
}

static int strcmp(const char *a, const char *b)
{
    while (*a && *b && *a == *b)
    {
        a++;
        b++;
    }
    return *a - *b;
}

static int cmd_is(const char *inp, const char *name)
{
    while (*name)
    {
        if (*inp++ != *name++)
            return 0;
    }
    return *inp == ' ' || *inp == '\0';
}

static char *cmd_arg(const char *cmd)
{
    while (*cmd && *cmd != ' ')
        ++cmd;
    if (*cmd == ' ')
        ++cmd;
    return (char *)cmd;
}

static uint64_t parse_hex(const char *s)
{
    if (s[0] == '0' && s[1] == 'x')
        s += 2;
    uint64_t val = 0;
    while (*s && *s != ' ')
    {
        char c = *s++;
        val <<= 4;
        if (c >= '0' && c <= '9')
            val |= c - '0';
        else if (c >= 'a' && c <= 'f')
            val |= c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            val |= c - 'A' + 10;
    }
    return val;
}

static void shell_exec(const char *cmd)
{
    if (cmd_is(cmd, "help"))
    {
        kprintf("Commands:\nhelp, clear, version, ticks, cr, mem, stack, uptime, outb, inb, rtc\n");
    }
    else if (cmd_is(cmd, "version"))
    {
        kprintf("Ados - v0.1\n");
    }
    else if (cmd_is(cmd, "clear"))
    {
        k_vga_clear(&kernel_vga);
    }
    else if (cmd_is(cmd, "ticks"))
    {
        kprintf("ticks: %llu\n", ticks);
    }
    else if (cmd_is(cmd, "cr"))
    {
        uint64_t cr0, cr2, cr3, cr4;
        __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
        __asm__ volatile("mov %%cr4, %0" : "=r"(cr4));
        kprintf("CR0: %llx\nCR2: %llx\nCR3: %llx\nCR4: %llx\n", cr0, cr2, cr3, cr4);
    }
    else if (cmd_is(cmd, "mem"))
    {
        const char *arg = cmd_arg(cmd);
        if (*arg == '\0')
        {
            kprintf("usage: mem 0xADDR\n");
            return;
        }

        uint64_t addr = parse_hex(arg);
        uint8_t *p = (uint8_t *)addr;

        for (int i = 0; i < 64; ++i)
        {
            if (i % 16 == 0)
                kprintf("\n%p: ", p + i);
            kprintf("%x ", p[i]);
        }
        kprintf("\n");
    }
    else if (cmd_is(cmd, "stack"))
    {
        uint64_t rsp;
        __asm__ volatile("mov %%rsp, %0" : "=r"(rsp));
        uint64_t *p = (uint64_t *)rsp;
        for (int i = 0; i < 16; ++i)
            kprintf("[rsp+%d]: %llx\n", i * 8, p[i]);
    }
    else if (cmd_is(cmd, "uptime"))
    {
        int dd, mm, ss;
        second_to_ddmmss(ticks / PIT_FREQ, &dd, &mm, &ss);
        kprintf("Uptime: %d:%d:%d\n", dd, mm, ss);
    }
    else if (cmd_is(cmd, "inb"))
    {
        char *arg1 = cmd_arg(cmd);
        if (*arg1 == '\0')
        {
            kprintf("Usage: inb 0xport 0xport\n");
            return;
        }
        uint16_t p1 = parse_hex(arg1);
        uint8_t val = inb(p1);
        kprintf("port 0x%x => 0x%x (%u)\n", p1, val, val);
    }
    else if (cmd_is(cmd, "outb"))
    {
        char *arg1 = cmd_arg(cmd);
        char *arg2 = cmd_arg(arg1);
        if (*arg1 == '\0' || *arg2 == '\0')
        {
            kprintf("Usage: outb 0xPORT 0xVAL\n");
            return;
        }
        int p1 = parse_hex(arg1);
        int p2 = parse_hex(arg2);
        outb(p1, p2);
        kprintf("0x%x => port 0x%x\n", p2, p1);
    }
    else if (cmd_is(cmd, "rtc"))
    {
        kprintf("Time: %x:%x:%x\n", cmos_read(0x04), cmos_read(0x02), cmos_read(0x00));
        kprintf("Date: %x/%x/%x\n", cmos_read(0x07), cmos_read(0x08), cmos_read(0x09));
    }
    else if (cmd[0] != '\0')
    {
        kprintf("unknown: %s\n", cmd);
    }
}

void shell_prompt() { kprintf("> "); }

void shell_feed(char c)
{
    if (c == '\n')
    {
        kprintf("\n");
        buff[buff_pos] = '\0';
        shell_exec(buff);
        buff_pos = 0;
        shell_prompt();
    }
    else if (c == '\b')
    {
        return;
        /*       if (buff_pos > 0) {
                   buf_pos
               }*/
    }
    else
    {
        if (buff_pos < BUFF_SIZE - 1)
        {
            buff[buff_pos++] = c;
            kprintf("%c", c);
        }
    }
}
