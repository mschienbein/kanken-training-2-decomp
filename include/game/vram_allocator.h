#ifndef GAME_VRAM_ALLOCATOR_H
#define GAME_VRAM_ALLOCATOR_H

/* [INFERENCE] Descriptive roles; bank probes and allocation clients establish
 * these widths and the complete 0x630-byte member at HeapManager +4. */
struct VramNode {
    unsigned char *address;
    unsigned size;
    struct VramNode *next;
};
struct VramAllocator {
    unsigned count[2];
    unsigned char *begin[2];
    unsigned size[2];
    struct VramNode nodes[2][65];
};

#endif
