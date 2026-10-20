#include "game/runtime.h"

struct IdentifierNode {
    int identifier;
    unsigned reserved04;
    struct IdentifierNode *next;
};
struct IdentifierRegistry {
    int count;
    unsigned char reserved04[0x14];
    struct IdentifierNode sentinel;
};

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
