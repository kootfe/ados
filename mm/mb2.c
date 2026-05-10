#include "mb2.h"
#include <stdint.h>

void mb2_parse_mmap(uint64_t mb2_phys, void (*cb)(uint64_t, uint64_t)) {
    uint8_t *p = (uint8_t *)(mb2_phys + 8);
    uint32_t total = *(uint32_t *)mb2_phys;
    uint8_t *end = (uint8_t*)mb2_phys + total;

    while (p < end) {
        mb2_tag_t *tag = (mb2_tag_t*)p;
        if (tag->type == 0) break;

        if (tag->type == MB2_TAG_MMAP) {
            mb2_tag_mmap_t *mt = (mb2_tag_mmap_t*)p;
            uint8_t *ep = p + sizeof(mb2_tag_mmap_t);
            uint8_t *eend = p + mt->size;

            while (ep < eend) {
                mb2_mmap_entry_t *e = (mb2_mmap_entry_t*)ep;
                if (e->type == 1)
                    cb(e->base, e->length);
                ep += mt->entry_size;
            }
        }
        
        p += (tag->size + 7) & ~7u;
    }
}
