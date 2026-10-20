#include "game/memory_arena.h"


void func_0010be94(void *memory, unsigned size)
{
    if (data_00242d74)
        return;
    data_00242d74 = 1;
    func_001927b8(&bss_004b204c, memory, size);
}
