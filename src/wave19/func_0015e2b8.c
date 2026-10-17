#include "game/task_runtime.h"

int func_0015e2b8(int index)
{
    const struct RuntimeActiveSlot *slot;
    int end;
    if (!bss_0029f540.active)
        return 0;
    if (-1 != index) {
        int next_index = 1 + index;
        end = next_index;
    } else {
        index = 0;
        end = 16;
    }
    slot = bss_0029f540.slots + index;
    for (; index < (int)end; ++index, ++slot)
        if (-1 != slot->identifier)
            return 1;
    return 0;
}
