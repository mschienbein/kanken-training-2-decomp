#include "game/audio_voice.h"

void func_001550e0(struct AudioChannel *channel,
                   const struct AdpcmCoefficients *coefficients)
{
    func_001550e8(channel->state, coefficients);
}
