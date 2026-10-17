#include "game/graphics_state.h"

signed char func_001228d4(unsigned int index)
{
    struct DisplayBuffer **buffers = func_001861c4()->buffers;
    return buffers[index]->active;
}
