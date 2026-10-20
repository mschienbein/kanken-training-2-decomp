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
    /* The allocator reuses the reset object's first storage word as a free
     * link; this is a pool-storage transition, not a different active receiver. */
    FileFreeEntry *entry = reinterpret_cast<FileFreeEntry *>(object);
    entry->next = bss_004b1d28.free;
    bss_004b1d28.free = entry;
    --bss_004b1d28.allocated;
}
