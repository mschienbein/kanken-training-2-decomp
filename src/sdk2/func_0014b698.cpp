#include "game/sound_runtime.h"
extern "C" bool func_0014b698(SoundFileBuffer *object, void *buffer, unsigned size)
{
    /* ORIGINAL BUG: the buffer end wraps if its address plus size overflows. */
    unsigned end = (unsigned)buffer + size;
    unsigned begin_address = ((unsigned)buffer + 31) & ~31u;
    if (begin_address > end)
        return false;
    object->begin = (unsigned char *)begin_address;
    object->end = (unsigned char *)end;
    object->current = (unsigned char *)begin_address;
    return true;
}
