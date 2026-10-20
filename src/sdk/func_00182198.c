#include "game/memory_arena.h"
#include "ctr/svc.h"



void func_00182198(struct MappedMemoryBlock *block)
{
    CtrHandle *handle;
    if (block->handle == 0)
        return;
    if (block->address != 0) {
        if (!block->mapped) {
            block->address = 0;
            block->size = 0;
        } else {
            svcUnmapMemoryBlock(block->handle, (unsigned)block->address);
            func_00148004(block);
        }
    }
    handle = &block->handle;
    if (*handle != 0) {
        svcCloseHandle(*handle);
        *handle = 0;
    }
}
