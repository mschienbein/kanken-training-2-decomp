#include "game/runtime.h"

struct OwnerIdentifierNode {
    int identifier;
    unsigned reserved04;
    struct OwnerIdentifierNode *next;
};
struct OwnerIdentifierRegistry {
    unsigned reserved00;
    int count;
    unsigned char reserved08[0x31c];
    struct OwnerIdentifierNode sentinel;
};

int func_0011774c(struct OwnerIdentifierRegistry *registry, int identifier)
{
    struct OwnerIdentifierNode *previous = &registry->sentinel;
    int *count = &registry->count;
    struct OwnerIdentifierNode *node;
    for (;;) {
        node = previous->next;
        if (node == 0) {
            return func_0019f568();
        }
        if (node->identifier == identifier)
            break;
        previous = node;
    }
    previous->next = node->next;
    node->identifier = 0;
    node->next = 0;
    node->reserved04 = 0;
    return --*count;
}
