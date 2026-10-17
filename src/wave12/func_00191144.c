#include "game/object_pool.h"
#include "game/pool_list.h"


struct PoolLink *func_00191144(struct ObjectPool *pool, struct PoolLink *node)
{
    return func_00191154(&pool->active, &pool->active.sentinel, node);
}
