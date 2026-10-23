#include "game/pool_list.h"

struct PoolLink *func_00191154(struct PoolList *list, struct PoolLink *position,
                              struct PoolLink *node)
{
    struct PoolLink *previous = position->previous;
    node->next = position;
    node->previous = previous;
    position->previous = node;
    previous->next = node;
    ++list->count;
    return node;
}
