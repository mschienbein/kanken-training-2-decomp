#include "game/audio_voice.h"
unsigned short func_001db054(struct HalfwordCursor001db054 *cursor)
{
    const unsigned short *current = cursor->current;
    unsigned short value = *current++;
    cursor->current = current;
    return value;
}
