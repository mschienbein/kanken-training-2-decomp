#include "game/audio_voice.h"

/* The channel/group callers consume this word as a sample position. */
unsigned func_001efcd8(const struct AudioChannelState *state)
{
    return state->value08;
}
