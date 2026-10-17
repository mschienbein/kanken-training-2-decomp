#include "game/audio_voice.h"
#include "game/stream_manager.h"

extern void func_00152664(struct AudioChannelState *state,
                          const struct AudioBiquadCoefficients *coefficients);

void func_0015263c(struct AudioChannel *channel,
                   const struct AudioBiquadCoefficients *coefficients)
{
    channel->filter = *coefficients;
    func_00152664(channel->state, &channel->filter);
}
