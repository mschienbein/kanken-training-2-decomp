#include "game/memory_arena.h"



/* Maps a block of the given size at page alignment from the global arena. */
void *func_0014aeb4(struct MappedMemoryBlock *block, unsigned int size)
{
    unsigned int alignment = 0x1000;
    struct MemoryArena *arena = &bss_004b51d0;
    return func_001906c8(arena, block, size, alignment);
}
