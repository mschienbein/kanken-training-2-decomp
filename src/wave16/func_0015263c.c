#include "game/functions.h"
#include "game/audio_voice.h"
#include "game/stream_manager.h"



void func_0015263c(struct AudioChannel *channel,
                   const struct AudioBiquadCoefficients *coefficients)
{
    channel->filter = *coefficients;
    func_00152664(channel->state, &channel->filter);
}
