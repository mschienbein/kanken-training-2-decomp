#include "game/sound.h"
#include "game/pool_list.h"





static __forceinline void setOwner(struct DeferredQueue *queue, struct DeferredEntry *entry)
{
    entry->owner = queue;
}

static __forceinline struct PoolLink *entryLink(struct DeferredEntry *entry)
{
    return &entry->link;
}

static __forceinline struct PoolIterator listEnd(struct PoolList *list)
{
    struct PoolIterator end;
    end.node = &list->sentinel;
    return end;
}

static __forceinline void enqueueEntry(struct DeferredEntry *entry, struct DeferredQueue *queue)
{
    setOwner(queue, entry);
    struct PoolList *list = &queue->active;
    struct PoolIterator position = listEnd(list);
    func_00191154(list, position.node, entryLink(entry));
}



void func_0014b5d8(struct DeferredQueue *queue, struct DeferredEntry *entry)
{
    enqueueEntry(entry, queue);
    ++queue->revision;
}
