#include "game/input_runtime.h"

int func_00151cf8(struct InputSnapshot *snapshot, signed char index)
{
    if (!snapshot->enabled || snapshot->selected != -1)
        return 0;
    if (index != 0 && index != 1)
        return 0;
    if (func_0015341c(func_001861e0(), 1, index)) {
        snapshot->selected = index;
        return 1;
    }
    return 0;
}
