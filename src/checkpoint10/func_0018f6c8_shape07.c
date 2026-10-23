#include "game/memory_arena.h"
#include "ctr/svc.h"
#include "game/waitable_handle.h"




void func_0018f6c8(struct MappedMemoryBlock *block)
{
    CtrHandle *handle;
    if (block->handle == 0)
        return;
    if (block->address != 0) {
        if (!block->mapped) {
            block->address = 0;
            block->size = 0;
        } else {
            CtrResult result;
            int failed;
            result = svcUnmapMemoryBlock(block->handle, (unsigned int)block->address);
            failed = result < 0;
            if (failed)
                func_0019dfbc(result);
            func_0014ae4c(&bss_004b51d0, block);
        }
    }
    handle = &block->handle;
    if (*handle != 0) {
        svcCloseHandle(*handle);
        *handle = 0;
    }
}
