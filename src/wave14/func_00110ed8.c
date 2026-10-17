#include "game/exp_heap.h"
#include "game/runtime.h"


void func_00110ed8(int count)
{
    int index;
    for (index = 0; index < 8; ++index) {
        bss_0049cb24.records[index].allocation = func_0019d798(1, 4, count * 8);
        bss_0049cb24.records[index].count = count;
    }
}
