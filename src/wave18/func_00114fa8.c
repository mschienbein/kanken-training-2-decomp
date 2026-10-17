#include "game/graphics_state.h"

extern unsigned func_00122a30(const struct GraphicsMutex *mutex, unsigned char value);

void func_00114fa8(void)
{
    if (data_002430f0.initialized)
        func_00122a30(&data_00243104, 0);
}
