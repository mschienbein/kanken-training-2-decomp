#include "game/exp_heap.h"



struct OwnedPoolObject;
struct OwnedPoolVTable {
    unsigned char reserved00[0x34];
    void (*reset)(OwnedPoolObject *);
};
struct OwnedObjectFreeList {
    unsigned char reserved00[0x24];
    OwnedPoolObject *free_head;
    unsigned int capacity;
    int used;
    StreamMutex mutex;
};
struct OwnedPool {
    unsigned char reserved00[0xb8];
    OwnedObjectFreeList objects;
};
struct OwnedPoolObject {
    union {
        const OwnedPoolVTable *vtable;
        OwnedPoolObject *next_free;
    };
    OwnedPool *owner;
    void release();
};

static void return_to_pool(OwnedObjectFreeList *pool, OwnedPoolObject *object)
{
    StreamLockGuard guard(&pool->mutex);
    object->next_free = pool->free_head;
    pool->free_head = object;
    --pool->used;
}

/* [INFERENCE] Descriptive class/member names; the image establishes the member ABI and pool operations. */
void OwnedPoolObject::release()
{
    OwnedPoolObject *object = this;
    object->vtable->reset(object);
    return_to_pool(&object->owner->objects, object);
}
