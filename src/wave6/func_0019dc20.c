#include "game/runtime.h"
struct HeapArena {
    unsigned char reserved00[0x54];
    unsigned allocations;
    unsigned char *lower;
    unsigned size;
};

struct HeapRange {
    unsigned char *lower;
    unsigned size;
};

struct HeapManager {
    unsigned char reserved00[0x640];
    struct HeapArena primary[3];
    unsigned reserved760[2];
    struct HeapRange ranges[3];
    struct HeapArena extra[2];
};

extern struct HeapManager bss_004952f0;
extern struct HeapArena bss_00495930;
extern struct HeapArena bss_00495990;
extern struct HeapArena bss_004959f0;
extern void func_00193a6c(struct HeapArena *heap, void *allocation);

void func_0019dc20(void *allocation)
{
    if (bss_004952f0.extra[0].lower <= (unsigned char *)allocation &&
        (unsigned char *)allocation < bss_004952f0.extra[0].lower + bss_004952f0.extra[0].size) {
        func_00193a6c(&bss_004952f0.extra[0], allocation);
        return;
    }
    if (bss_004952f0.extra[1].lower <= (unsigned char *)allocation &&
        (unsigned char *)allocation < bss_004952f0.extra[1].lower + bss_004952f0.extra[1].size) {
        func_00193a6c(&bss_004952f0.extra[1], allocation);
        return;
    }
    if (bss_004952f0.ranges[0].lower <= (unsigned char *)allocation &&
        (unsigned char *)allocation < bss_004952f0.ranges[0].lower + bss_004952f0.ranges[0].size) {
        func_00193a6c(&bss_00495930, allocation);
        return;
    }
    if (bss_004952f0.ranges[1].lower <= (unsigned char *)allocation &&
        (unsigned char *)allocation < bss_004952f0.ranges[1].lower + bss_004952f0.ranges[1].size) {
        func_00193a6c(&bss_00495990, allocation);
        return;
    }
    if (bss_004952f0.ranges[2].lower <= (unsigned char *)allocation &&
        (unsigned char *)allocation < bss_004952f0.ranges[2].lower + bss_004952f0.ranges[2].size)
        func_00193a6c(&bss_004959f0, allocation);
}
