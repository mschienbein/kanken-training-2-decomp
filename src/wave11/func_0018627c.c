#include "game/exp_heap.h"
#include <stddef.h>
void *func_0018627c(const struct IntrusiveList *list, const void *object)
{
    if (!object)
        return list->head;
    /* Generic intrusive lists select a link by a runtime offset, not a fixed field. */
    return *(void *const *)(list->link_offset +
                            offsetof(struct IntrusiveLink, next) + (unsigned)object);
}
