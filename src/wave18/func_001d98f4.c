#include "game/functions.h"
#include "game/audio_voice.h"
/* [INFERENCE] Per-channel modulation state; untouched prefix belongs to its configuration. */
void func_001d98f4(struct AudioModulationState *state)
{
    state->value = 0.0f;
    state->scale = 1.0f;
    state->elapsed = 0;
    state->active = 0;
    state->pending = 0;
}
