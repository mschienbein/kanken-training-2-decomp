#include "game/audio_voice.h"
#include "game/stream_manager.h"

void func_00152664(struct AudioChannelState *state,
                   const struct AudioBiquadCoefficients *coefficients)
{
    state->biquad = *coefficients;
    state->flags |= 0x10;
}
