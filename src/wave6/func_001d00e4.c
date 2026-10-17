#include <stddef.h>

#include "game/pool_list.h"

struct DeferredQueue {
    unsigned char reserved00[0x18];
    struct PoolList active;
    unsigned char reserved24[0x14];
    unsigned int revision;
};

struct DeferredEntry {
    unsigned char reserved00[8];
    struct DeferredQueue *owner;
    unsigned char reserved0c[0x10];
    struct PoolLink link;
};


struct DeferredEntry *func_001d00e4(struct DeferredQueue *queue)
{
    struct DeferredEntry *entry;
    struct PoolLink *first;
    if (queue->active.count == 0)
        return 0;
    first = queue->active.sentinel.next;
    entry = (struct DeferredEntry *)((char *)first - offsetof(struct DeferredEntry, link));
    func_00191178(&queue->active, first);
    return entry;
}
