#include "game/audio_voice.h"
unsigned char func_001db068(struct ByteCursor001db068 *cursor)
{
    const unsigned char *current = cursor->current;
    unsigned char value = *current++;
    cursor->current = current;
    return value;
}
