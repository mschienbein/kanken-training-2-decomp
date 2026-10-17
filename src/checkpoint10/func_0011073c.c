#include "game/memory_arena.h"
#include "ctr/svc.h"
#include "game/waitable_handle.h"

/* Shared mapped-block layout copied exactly from src/sdk/func_00136754.c. */
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
extern unsigned char data_00242d74;



static void *allocate_block_pages(struct MappedMemoryBlock *block, unsigned size)
{
    return func_001906c8(&bss_004b204c, block, size, 0);
}

void func_0011073c(struct MappedMemoryBlock *block, unsigned int size)
{
    if (block == 0)
        return;
    block->next = 0;
    block->previous = 0;
    block->address = 0;
    block->size = 0;
    block->read_only = 0;
    if (data_00242d74) {
        const unsigned rounded = (size + 0xfffu) & ~0xfffu;
        if (allocate_block_pages(block, rounded) == 0)
            func_0019dfbc((CtrResult)0xd8601837u);
        block->read_only = 0;
    }
}
