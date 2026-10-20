#include "game/exp_heap.h"
#include <stddef.h>
#include "game/runtime.h"



extern "C" AllocatorBase *func_00183234(AllocatorBase *allocator)
{
    allocator->vtable = data_00223c98;
    if (allocator->outstanding != 0)
        func_0019f568();
    if (allocator->children != 0)
        func_0019f568();
    AllocatorLink **head = &allocator->children;
    AllocatorLink *link = *head;
    if (link != 0) {
        do {
            AllocatorLink *current = link;
            link = link->next;
            current->next = 0;
            current->previous = 0;
        } while (link != 0);
        *head = 0;
    }
    return reinterpret_cast<AllocatorBase *>(reinterpret_cast<char *>(head) -
                                            offsetof(AllocatorBase, children));
}
