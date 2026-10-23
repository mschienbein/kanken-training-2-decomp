#include "game/file_handle.h"
#include "game/file_session.h"
#include "game/stream_manager.h"




extern "C" void func_001c4bdc(FileSessionObject *object)
{
    if (object->handle != 0) {
        CtrHandle session = func_0016bd7c(object);
        func_001c4f80(&session);
    }
    object->vtable->reset(object);
    StreamLockGuard guard(&bss_004b1d28.mutex);
    /* The reset object enters the pool's explicit free-link storage state. */
    object->next_free = bss_004b1d28.free;
    bss_004b1d28.free = object;
    --bss_004b1d28.allocated;
}
