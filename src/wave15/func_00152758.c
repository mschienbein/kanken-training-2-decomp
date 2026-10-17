#include "game/audio_voice.h"

void func_00152758(struct AudioChannelState *state, float value)
{
    state->value68 = value;
    state->flags |= 1;
}
