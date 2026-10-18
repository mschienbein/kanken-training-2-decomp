#include "game/stream_manager.h"

struct GlobalPoolGuard { StreamMutex *mutex; };
static __forceinline void acquire_pool(GlobalPoolGuard *guard, StreamMutex *mutex)
{
    guard->mutex = mutex;
    func_00192bd8(guard->mutex);
}
static __forceinline void release_pool(GlobalPoolGuard *guard)
{
    func_00195d74(guard->mutex);
}

struct GlobalPoolObject;
struct GlobalPoolVTable {
    unsigned char reserved00[0x34];
    void (*reset)(GlobalPoolObject *);
};
struct GlobalPoolObject {
    union {
        const GlobalPoolVTable *vtable;
        GlobalPoolObject *next_free;
    };
    void release();
};
struct GlobalObjectFreeList {
    unsigned char reserved00[0x24];
    GlobalPoolObject *free_head;
    unsigned int capacity;
    int used;
    StreamMutex mutex;
};
/* INTEGRATOR: move to include/game/file_session.h */
extern "C" {
extern GlobalObjectFreeList bss_004b1cec;
}

/* [INFERENCE] Descriptive class/member names; the image establishes the member ABI and pool operations. */
void GlobalPoolObject::release()
{
    GlobalPoolObject *object = this;
    object->vtable->reset(object);
    GlobalPoolGuard guard;
    acquire_pool(&guard, &bss_004b1cec.mutex);
    object->next_free = bss_004b1cec.free_head;
    bss_004b1cec.free_head = object;
    --bss_004b1cec.used;
    release_pool(&guard);
}
