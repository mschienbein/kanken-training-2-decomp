#include "ctr/svc.h"

struct MemoryBlock {
    struct MemoryBlock *next;
    struct MemoryBlock *previous;
    unsigned int address;
    unsigned int size;
    unsigned int reserved10;
    CtrHandle handle;
    unsigned char mapped;
};

extern void func_00148004(struct MemoryBlock *block);

void func_00182198(struct MemoryBlock *block)
{
    CtrHandle *handle;
    if (block->handle == 0)
        return;
    if (block->address != 0) {
        if (!block->mapped) {
            block->address = 0;
            block->size = 0;
        } else {
            svcUnmapMemoryBlock(block->handle, block->address);
            func_00148004(block);
        }
    }
    handle = &block->handle;
    if (*handle != 0) {
        svcCloseHandle(*handle);
        *handle = 0;
    }
}
