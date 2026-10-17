#include "game/functions.h"


/* [INFERENCE] The retained next pointer is represented as a successor iterator;
 * observed callers ignore the result. */
struct RegistryNode *func_0018d868(struct Registry *registry, struct RegistryNode *node)
{
    struct RegistryNode *next = node->next;
    struct RegistryNode *previous = node->previous;
    next->previous = previous;
    previous->next = next;
    --registry->count;
    node->next = 0;
    node->previous = 0;
    return next;
}
