#ifndef PMM_H
#define PMM_H
    
#include <stdint.h>

#define PAGE_SIZE 4096ULL
#define PAGE_SHIFT 12
#define MAX_ORDER 11 // 0..11

void pmm_init(uint64_t mb2_phys);

uint64_t pmm_alloc(int order);
void pmm_free(uint64_t addr, int order);
void pmm_dump();

#endif
