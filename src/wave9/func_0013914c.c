#include "game/sound_format.h"

unsigned char func_0013914c(const struct WaveNoteInfo *note)
{
    unsigned value;
    if (!func_0018279c(&note->flags, &value, 1))
        return 96;
    return (unsigned char)value;
}
