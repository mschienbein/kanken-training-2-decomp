#include "game/functions.h"

struct PoolPosition {
    struct PoolLink *node;
    PoolPosition(struct PoolLink *value) : node(value) {}
};

/* Codegen qualification: this value-position adapter preserves the original
 * three-word forwarding while retaining the shared pointer-based insertion
 * interface. Its source-level presence in the original is [INFERENCE]. */
static __forceinline struct PoolLink *insert_position(struct PoolList *registry,
    struct PoolPosition position, struct PoolLink *node)
{
    return func_00191154(registry, position.node, node);
}

extern "C" struct PoolLink *func_0017e790(struct PoolList *registry, struct RegistryEntry *entry)
{
    return insert_position(registry, PoolPosition(&registry->sentinel), &entry->link);
}
