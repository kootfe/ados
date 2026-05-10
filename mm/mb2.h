#ifndef MB2_H
#define MB2_H

#include <stdint.h>

#define MB2_TAG_MMAP 6

typedef struct mb2_tag
{
    uint32_t type;
    uint32_t size;
} __attribute__((packed)) mb2_tag_t;

typedef struct mb2_tag_mmap
{
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
} __attribute__((packed)) mb2_tag_mmap_t;

typedef struct mb2_mmap_entry
{
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} __attribute__((packed)) mb2_mmap_entry_t;

void mb2_parse_mmap(uint64_t mb2_phys, void (*cb)(uint64_t base, uint64_t len));

#endif
