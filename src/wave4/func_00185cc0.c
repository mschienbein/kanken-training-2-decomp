#include "game/functions.h"

struct PoolLink {
    struct PoolLink *next;
    struct PoolLink *previous;
};

struct PoolList {
    unsigned int count;
    struct PoolLink sentinel;
};

struct ObjectPool {
    void *free_list;
    struct PoolList active;
    unsigned char initialized;
    unsigned char reserved11[3];
    unsigned int capacity;
    void *storage;
    unsigned int storage_size;
};

extern volatile unsigned int data_00243718;
extern struct ObjectPool bss_004b8420;
extern int func_00191c80(volatile unsigned int *guard);

static __forceinline void initializeList(struct PoolList *list)
{
    list->count = 0;
    list->sentinel.next = &list->sentinel;
    list->sentinel.previous = &list->sentinel;
}

static __forceinline void initializePool(struct ObjectPool *pool)
{
    pool->free_list = 0;
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
