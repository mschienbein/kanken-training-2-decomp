#include "game/functions.h"


/* [INFERENCE] The retained next pointer is represented as a successor iterator;
 * observed callers ignore the result. */
struct PoolLink *func_0018d868(struct PoolList *registry, struct PoolLink *node)
{
    struct PoolLink *next = node->next;
    struct PoolLink *previous = node->previous;
    next->previous = previous;
    previous->next = next;
    --registry->count;
    node->next = 0;
    node->previous = 0;
    return next;
}
