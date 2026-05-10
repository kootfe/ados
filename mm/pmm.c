#include "pmm.h"
#include "../io/term.h"
#include "mb2.h"
#include <exception>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define MAX_PAGES (1024 * 1024)

typedef struct block
{
    struct block *next;
    struct block *prev;
} block_t;

static block_t *free_list[MAX_ORDER];
static uint64_t free_count[MAX_ORDER];

static uint64_t bitmap[MAX_PAGES / 64];

static inline void bit_set(uint64_t pfn) { bitmap[pfn >> 6] |= (1ULL << (pfn & 63)); }

static inline void bit_clear(uint64_t pfn) { bitmap[pfn >> 6] &= ~(1ULL << (pfn & 63)); }

static inline int bit_get(uint64_t pfn) { return (bitmap[pfn >> 6] >> (pfn & 63)) & 1; }

static void list_push(int order, block_t *b)
{
    b->next = free_list[order];
    b->prev = NULL;
    if (free_list[order])
        free_list[order]->prev = b;
    free_list[order] = b;
    free_count[order]++;
}

static void list_remove(int order, block_t *b)
{
    if (b->prev)
        b->prev->next = b->next;
    else
        free_list[order] = b->next;
    if (b->next)
        b->next->prev = b->prev;
    b->next = b->prev = NULL;
    free_count[order]--;
}

static inline uint64_t buddy_of(uint64_t addr, int order) { return addr ^ (PAGE_SIZE << order); }

extern uint8_t __kernel_start[]; // linker script
extern uint8_t __kernel_end[];

static inline int overlaps_kernel(uint64_t base, uint64_t len)
{
    uint64_t ks = (uint64_t)__kernel_start;
    uint64_t ke = (uint64_t)__kernel_end;
    return !(base + len <= ks || base >= ke);
}

static void pmm_add_region(uint64_t base, uint64_t len)
{
    uint64_t aligned = (base + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    if (aligned >= base + len)
        return;

    len -= (aligned - base);
    base = aligned;
    len = len & ~(PAGE_SIZE - 1);

    if (base == 0)
    {
        if (len <= PAGE_SIZE)
            return;
        base += PAGE_SIZE;
        len -= PAGE_SIZE;
    }

    while (len >= PAGE_SIZE)
    {
        if (overlaps_kernel(base, PAGE_SIZE))
        {
            base += PAGE_SIZE;
            len -= PAGE_SIZE;
            continue;
        }

        int order = MAX_ORDER - 1;
        while (order > 0)
        {
            uint64_t block_size = PAGE_SIZE << order;
            if (len >= block_size && (base & (block_size - 1)) == 0)
                break;
            order--;
        }

        pmm_free(base, order);
        uint64_t sz = PAGE_SIZE << order;
        base += sz;
        len -= sz;
    }
}

static uint64_t s_mb2_phys;

static void region_cb(uint64_t base, uint64_t len)
{
    kprintf("  mmap region: %llx + %llx\n", base, len);
    uint64_t mb2_end = s_mb2_phys + 4096;
    if (s_mb2_phys >= base + len || mb2_end <= base)
    {
        pmm_add_region(base, len);
        return;
    }

    if (base < s_mb2_phys)
        pmm_add_region(base, s_mb2_phys - base);

    if (mb2_end < base + len)
        pmm_add_region(base, (base + len) - mb2_end);
}
