#include "game/object_pool.h"

struct PoolStorage { void *memory; unsigned size; };
static unsigned populate_pool(struct FreeListPool *pool, struct PoolStorage storage)
{
    return func_00180768(pool, storage.memory, storage.size, 0x1b8);
}
void func_00147264(struct ObjectPool *pool, void *storage, unsigned size)
{
    struct PoolStorage backing;
    backing.memory = storage;
    backing.size = size;
    if (!pool->initialized) {
        unsigned count = populate_pool(&pool->free_list, backing);
        pool->storage = backing.memory;
        pool->capacity = count;
        pool->initialized = 1;
        pool->storage_size = backing.size;
    }
}
