#include "ctr/svc.h"

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
extern void *func_001906c8(struct MemoryArena *arena, struct MappedMemoryBlock *block,
                           unsigned int size, unsigned int alignment);

static __forceinline void publishMapping(CtrResult result, struct MappedMemoryBlock *block)
{
    unsigned int failure = (unsigned int)result >> 31;
    if (failure == 0)
        block->mapped = 1;
}

CtrResult func_00136754(struct MappedMemoryBlock *block, unsigned int size,
                       unsigned char read_only)
{
    void *address;
    CtrResult result;

    if (block->address != 0)
        return (CtrResult)0x08a01bf9u;
    if ((size & 0xfffu) != 0)
        return (CtrResult)0xe0e01bf2u;
    address = func_001906c8(&bss_004b51d0, block, size, 0x1000);
    if (address == 0)
        return (CtrResult)0xd8601837u;
    block->read_only = read_only;
    result = svcMapMemoryBlock(block->handle, (unsigned int)address,
                               read_only ? 1u : 3u, 0x10000000u);
    publishMapping(result, block);
    return result;
}
