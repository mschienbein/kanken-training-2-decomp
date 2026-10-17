#include "game/graphics_state.h"

extern unsigned func_0012f9a8(struct GraphicsMutex *mutex, unsigned register_address,
                              void *destination, unsigned size);

unsigned func_00122894(unsigned register_address, void *destination, unsigned size)
{
    return func_0012f9a8(func_001861d4(), register_address, destination, size);
}
