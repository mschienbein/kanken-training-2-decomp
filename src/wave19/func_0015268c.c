#include "game/audio_voice.h"

void func_0015268c(struct AudioChannelState *state, float value)
{
    state->value24 = value >= 0.0f ? value : 0.0f;
    state->flags |= 2;
}
