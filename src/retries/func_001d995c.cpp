#include "game/cleanup_registry.h"
#include <new>
#include "game/pool_list.h"



static inline void register_callback(PoolList *list, PoolLink *node)
{
    func_00191154(list, &list->sentinel, node);
}

void *func_001d995c(CleanupContext *context, unsigned size,
                  CleanupCallback callback, void *cookie)
{
    void *allocation = func_001383e8(context->heap,
        ((size + 31) & ~31u) + sizeof(CleanupNode), 32);
    if (allocation == 0)
        return 0;
    void *memory = (CleanupNode *)allocation + 1;
    CleanupNode *node = new (allocation) CleanupNode(memory, size, callback, cookie);
    CleanupGroup *group = (CleanupGroup *)context->groups.sentinel.previous;
    register_callback(&group->callbacks, &node->link);
    return memory;
}
