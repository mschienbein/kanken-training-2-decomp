#include "game/functions.h"

extern void func_0018d868(struct Registry *registry, struct RegistryNode *node);

void func_0017e7bc(struct Registry *registry, struct RegistryEntry *entry)
{
    func_0018d868(registry, &entry->link);
}
