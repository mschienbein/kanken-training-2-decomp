#include "game/exp_heap.h"
struct HeapBase *func_00186224(const struct IntrusiveList *list, const void *memory)
{
    struct HeapBase *heap = 0;
    for (;;) {
        struct HeapBase *child;
        heap = func_0018627c(list, heap);
        if (!heap)
            return heap;
        if ((unsigned)memory < (unsigned)heap->begin ||
            (unsigned)heap->end <= (unsigned)memory)
            continue;
        child = func_00186224(&heap->children, memory);
        return child ? child : heap;
    }
}
