#include "game/heap_manager.h"

/* Both virtual inputs are retained: this allocation slot ignores the Heap
 * receiver and the requested alignment, and dispatches through the singleton. */
extern "C" void *func_001e37b8(Heap *allocator, unsigned size, unsigned alignment)
{
    return func_001e35e4(&bss_004952f0, 0x10000, 0x100, 0, size);
}
