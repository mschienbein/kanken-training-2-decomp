#include "game/functions.h"

#include "game/object_pool.h"

static __forceinline void initializeList(struct PoolList *list)
{
    list->count = 0;
    list->sentinel.next = &list->sentinel;
    list->sentinel.previous = &list->sentinel;
}

static __forceinline void initializePool(struct ObjectPool *pool)
{
    pool->free_list.head = 0;
    initializeList(&pool->active);
    pool->initialized = 0;
    pool->capacity = 0;
}

struct ObjectPool *func_00185cc0(void)
{
    if (!(data_00243718 & 1) && func_00191c80(&data_00243718)) {
        initializePool(&bss_004b8420);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_00243718);
    }
    return &bss_004b8420;
}
