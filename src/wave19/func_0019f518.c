#include "game/object_pool.h"

void func_0019f518(struct IdentifierRegistry *registry, int identifier)
{
    struct IdentifierNode *previous = &registry->sentinel;
    struct IdentifierNode *node;
    for (;;) {
        node = previous->next;
        if (node == 0) {
            func_0019f568();
            return;
        }
        if (node->identifier == identifier)
            break;
        previous = node;
    }
    previous->next = node->next;
    node->identifier = 0;
    node->next = 0;
    node->reserved04 = 0;
    --registry->count;
}
