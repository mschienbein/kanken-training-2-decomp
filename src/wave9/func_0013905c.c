#include "game/sound_format.h"

unsigned char func_0013905c(const struct WaveNoteInfo *note)
{
    unsigned value;
    if (!func_0018279c(&note->flags, &value, 0))
        return 64;
    return (unsigned char)value;
}
