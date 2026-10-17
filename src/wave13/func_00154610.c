#include "game/audio_voice.h"

void func_00154610(struct AudioChannelState *state, unsigned int value)
{
    state->flags1e = (state->flags1e & ~0x40) | (value << 6);
}
