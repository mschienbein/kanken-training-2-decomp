#include "game/audio_voice.h"
void func_00152610(struct AudioChannelState *state, unsigned char value)
{
    state->flag0f = value;
    state->flags |= 4;
}
