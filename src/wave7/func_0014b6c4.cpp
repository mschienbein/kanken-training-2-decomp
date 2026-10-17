#include "game/sound_runtime.h"

extern "C" SoundFileBuffer *func_0014b6c4(SoundFileBuffer *object)
{
    object->reserved04 = 0;
    object->vtable = data_002245d8;
    object->owner = 0;
    object->begin = 0;
    object->end = 0;
    object->current = 0;
    object->active = false;
    object->link.next = 0;
    object->link.previous = 0;
    return object;
}
