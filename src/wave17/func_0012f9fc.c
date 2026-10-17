#include "game/graphics_state.h"

extern int func_0012fa24(struct GraphicsMutex *mutex, unsigned register_address,
                         const void *source, unsigned size);

int func_0012f9fc(unsigned register_address, const void *source, unsigned size)
{
    return func_0012fa24(func_001861d4(), register_address, source, size);
}
