#include "game/audio_voice.h"
void func_00168cd8(struct SequenceEnabledState *state, int enabled)
{
    if (enabled)
        enabled = 0;
    else
        enabled = 1;
    state->disabled = enabled;
}
