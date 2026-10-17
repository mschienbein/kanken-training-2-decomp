#include "game/sound_track.h"
/* [INFERENCE] The two words are a limit and current byte position. */

unsigned int func_001f18a8(const struct BufferPosition *buffer)
{
    return buffer->limit - buffer->position;
}
