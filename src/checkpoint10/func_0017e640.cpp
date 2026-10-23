#include "game/object_manager.h"


struct RegistryIterator {
    struct PoolLink *node;
    RegistryIterator(struct PoolLink *value) : node(value) {}
};
static inline struct PoolLink *Insert(struct PoolList *list, RegistryIterator position, struct PoolLink *node)
{
    return func_00191154(list, position.node, node);
}
extern "C" struct PoolLink *func_0017e640(struct ObjectManager *manager, struct RegistryEntry *entry)
{
    struct PoolList *list = &manager->active;
    RegistryIterator position(&list->sentinel);
    return Insert(list, position, &entry->link);
}
