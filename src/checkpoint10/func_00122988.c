#include "game/graphics_state.h"

extern int func_0012fad8(struct GraphicsMutex *mutex, unsigned register_address,
                         const void *source, const void *mask, unsigned size);

int func_00122988(unsigned register_address, const void *source,
                  const void *mask, unsigned size)
{
    return func_0012fad8(func_001861d4(), register_address, source, mask, size);
}
