#include "game/heap_manager.h"
extern struct HeapArena bss_00495930;
extern struct HeapArena bss_00495990;
extern struct HeapArena bss_004959f0;

void func_0019dc20(void *allocation)
{
    if (bss_004952f0.extra[0].lower <= (unsigned char *)allocation &&
        (unsigned char *)allocation < bss_004952f0.extra[0].lower + bss_004952f0.extra[0].size) {
        func_00193a6c(&bss_004952f0.extra[0].base, allocation);
        return;
    }
    if (bss_004952f0.extra[1].lower <= (unsigned char *)allocation &&
        (unsigned char *)allocation < bss_004952f0.extra[1].lower + bss_004952f0.extra[1].size) {
        func_00193a6c(&bss_004952f0.extra[1].base, allocation);
        return;
    }
    if (bss_004952f0.ranges[0].lower <= (unsigned char *)allocation &&
        (unsigned char *)allocation < bss_004952f0.ranges[0].lower + bss_004952f0.ranges[0].size) {
        func_00193a6c(&bss_00495930.base, allocation);
        return;
    }
    if (bss_004952f0.ranges[1].lower <= (unsigned char *)allocation &&
        (unsigned char *)allocation < bss_004952f0.ranges[1].lower + bss_004952f0.ranges[1].size) {
        func_00193a6c(&bss_00495990.base, allocation);
        return;
    }
    if (bss_004952f0.ranges[2].lower <= (unsigned char *)allocation &&
        (unsigned char *)allocation < bss_004952f0.ranges[2].lower + bss_004952f0.ranges[2].size)
        func_00193a6c(&bss_004959f0.base, allocation);
}
