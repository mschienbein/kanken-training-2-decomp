#include "game/stream_manager.h"
#include "game/file_session.h"

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
