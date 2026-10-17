#include <string.h>
#include "game/audio_voice.h"

void func_001550e8(struct AudioChannelState *state,
                   const struct AdpcmCoefficients *coefficients)
{
    memcpy(&state->coefficients, coefficients, sizeof(*coefficients));
    state->flags |= 0x40;
}
