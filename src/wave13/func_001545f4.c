#include "game/audio_voice.h"

void func_001545f4(struct AudioChannelState *state, int mode)
{
    state->flags1e = (state->flags1e & ~0x10) | (mode << 4);
}
