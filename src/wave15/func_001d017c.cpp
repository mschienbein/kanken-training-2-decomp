#include "game/sound_controller.h"

/* The derived teardown at func_001d40a8 tail-calls this entry. */

extern "C" ResourceState001d0164 *func_001d017c(ResourceState001d0164 *object)
{
    object->vtable = data_002244b4;
    return object;
}
