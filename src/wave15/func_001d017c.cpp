#include "game/sound_types.h"

/* The derived teardown at func_001d40a8 tail-calls this entry. */

extern "C" W15iSoundType *func_001d017c(W15iSoundType *object)
{
    object->vtable = data_002244b4;
    return object;
}
