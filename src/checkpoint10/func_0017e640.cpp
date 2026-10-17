#include "game/functions.h"

struct ManagerLock {
    unsigned int state;
};
/* INTEGRATOR: differs from src/wave4/func_001470c8.c: active registry is evidenced at 0x1dc beyond its partial manager layout. */
struct ObjectManager {
    unsigned char reserved00[0x1c4];
    struct ManagerLock lock;
    unsigned char reserved1c8[0x14];
    struct PoolList active;
};


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
