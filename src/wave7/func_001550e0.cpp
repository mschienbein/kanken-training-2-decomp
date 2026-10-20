#include "game/audio_voice.h"

extern "C" void func_001550e0(struct AudioChannel *channel,
                   const struct AdpcmCoefficients *coefficients)
{
    func_001550e8(channel->state, coefficients);
}
