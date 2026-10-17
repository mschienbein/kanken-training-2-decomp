#include "game/memory_arena.h"
/* Only the allocator address is used here; its lock/layout is not recovered. */
struct MemoryArena;
extern unsigned char data_00242d74;


void func_0010be94(void *memory, unsigned size)
{
    if (data_00242d74)
        return;
    data_00242d74 = 1;
    func_001927b8(&bss_004b204c, memory, size);
}
