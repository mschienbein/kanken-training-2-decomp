#include "ctr/svc.h"
#include "game/waitable_handle.h"

/* Exact shared mapping layout from src/sdk/func_00136754.c. */
struct MappedMemoryBlock {
    struct MappedMemoryBlock *next;
    struct MappedMemoryBlock *previous;
    void *address;
    unsigned int size;
    unsigned char read_only;
    unsigned char reserved[3];
    CtrHandle handle;
    unsigned char mapped;
};

struct MemoryArena;
extern struct MemoryArena bss_004b51d0;
extern void func_0014ae4c(struct MemoryArena *arena, struct MappedMemoryBlock *block);

/* INTEGRATOR: differs from src/sdk/func_001c51a4.c: the pointed object is a
 * MappedMemoryBlock, evidenced by its mapping fields and shared arena; the
 * caller's opaque ThreadState view has the same handle offset. */
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
