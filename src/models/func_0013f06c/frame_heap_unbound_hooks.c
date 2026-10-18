#include "game/region_heap.h"

/* [INFERENCE] Empty release-mode heap hooks were inlined by armlink and
 * removed by --remove. The original sites establish neither identity nor
 * original address. The heap pointer is forwarded by the caller; neither
 * candidate hook accesses it. These are natural, unbound C definitions. */
void frame_heap_release_lock(void *heap)
{
    (void)heap;
}

void frame_heap_release_unlock(void *heap)
{
    (void)heap;
}
