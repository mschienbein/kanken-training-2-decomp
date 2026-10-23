#include "game/exp_heap.h"

/* [INFERENCE] Private default-allocation policy; historical helper/class identity
 * is unrecovered. Every field supplies one real canonical allocator argument. */
struct HeapAllocationPolicy {
    ManagedExpHeap *heap;
    int group, direction, mode;
    HeapAllocationPolicy(ManagedExpHeap &value, int initial_direction, int initial_group, int initial_mode)
        : heap(&value), group(initial_group), direction(initial_direction), mode(initial_mode) {}
    void *allocate(unsigned size, int alignment) const {
        return func_00195104(heap, size, alignment, group, direction, mode);
    }
};

/* [INFERENCE] A virtual allocator slot: the instance is unused because this
 * implementation delegates to the singleton heap; size and alignment pass through. */
void *func_001e37ec(AllocatorInterface *instance, unsigned int size, int alignment)
{
    const HeapAllocationPolicy policy(bss_00495ad0.base, 0, 0, 0);
    return policy.allocate(size, alignment);
}
