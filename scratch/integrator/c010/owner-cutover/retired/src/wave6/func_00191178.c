#include "game/pool_list.h"

static __forceinline struct PoolLink *eraseRange(struct PoolList *list,
                                               struct PoolLink *first,
                                               struct PoolLink *last)
{
    while (first != last) {
        struct PoolLink *previous = first->previous;
        struct PoolLink *next = first->next;
        next->previous = previous;
        previous->next = next;
        --list->count;
        first->next = 0;
        first->previous = 0;
        first = next;
    }
    return last;
}

struct PoolLink *func_00191178(struct PoolList *list, struct PoolLink *first)
{
    return eraseRange(list, first, first->next);
}
