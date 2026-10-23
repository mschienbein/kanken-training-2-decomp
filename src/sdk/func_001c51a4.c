#include <stddef.h>
#include "ctr/svc.h"
#include "game/memory_arena.h"


struct MappedMemoryBlock *func_001c51a4(struct MappedMemoryBlock *object)
{
    CtrHandle *handle;
    func_0018f6c8(object);
    handle = &object->handle;
    if (*handle != 0) {
        svcCloseHandle(*handle);
        *handle = 0;
    }
    return (struct MappedMemoryBlock *)((unsigned char *)handle -
                                        offsetof(struct MappedMemoryBlock, handle));
}
