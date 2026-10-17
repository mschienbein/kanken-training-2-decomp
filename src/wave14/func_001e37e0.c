#include "game/exp_heap.h"
/* [INFERENCE] The allocator vtable supplies the unused interface pointer. */
void func_001e37e0(struct AllocatorInterface *interface, void *allocation)
{
    func_00193a6c(&bss_00495ad0.base, allocation);
}
