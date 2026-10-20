#include "game/file_handle.h"
#include "game/file_session.h"
#include "game/stream_manager.h"

struct FileSessionVTable {
    void (*reserved00[13])(void);
    FileSessionObject *(*reset)(FileSessionObject *object);
};
struct FileFreeEntry { FileFreeEntry *next; };
struct FileObjectPool {
    unsigned char reserved00[0x24];
    FileFreeEntry *free;
    unsigned reserved28;
    unsigned allocated;
    StreamMutex mutex;
};



extern "C" void func_001c4bdc(FileSessionObject *object)
{
    if (object->handle != 0) {
        CtrHandle session = func_0016bd7c(object);
        func_001c4f80(&session);
    }
    object->vtable->reset(object);
    StreamLockGuard guard(&bss_004b1d28.mutex);
    FileFreeEntry *entry = reinterpret_cast<FileFreeEntry *>(object);
    entry->next = bss_004b1d28.free;
    bss_004b1d28.free = entry;
    --bss_004b1d28.allocated;
}
