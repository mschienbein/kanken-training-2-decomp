#include "game/sound_format.h"

unsigned char func_001390b0(const struct WaveNoteInfo *note)
{
    unsigned value;
    if (!func_0018279c(&note->flags, &value, 2))
        return 64;
    return (unsigned char)value;
}
