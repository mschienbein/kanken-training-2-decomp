#include "game/exp_heap.h"

extern struct IntrusiveList bss_004b96ec;
extern struct HeapBase *func_00186224(const struct IntrusiveList *list, const void *memory);
extern void func_0012edd4(struct IntrusiveList *list, void *object);

/* One level of the heap-containment search, inlined into its caller; the
 * recursion into child heaps is the out-of-line func_00186224. */
static struct HeapBase *find_contain(const struct IntrusiveList *list, const void *memory)
{
    struct HeapBase *heap = 0;
    for (;;) {
        struct HeapBase *child;
        heap = func_0018627c(list, heap);
        if (!heap)
            return 0;
        if ((unsigned)memory < (unsigned)heap->begin || (unsigned)heap->end <= (unsigned)memory)
            continue;
        child = func_00186224(&heap->children, memory);
        if (!child)
            child = heap;
        return child;
    }
}

/* Unlink a heap from its containing heap's child list, or from the root list. */
void func_00121bc8(struct HeapBase *heap)
{
    struct IntrusiveList *list = &bss_004b96ec;
    struct HeapBase *contain = find_contain(&bss_004b96ec, heap);
    if (contain)
        list = &contain->children;
    func_0012edd4(list, heap);
}
