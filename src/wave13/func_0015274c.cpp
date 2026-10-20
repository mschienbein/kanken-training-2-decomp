#include "game/audio_voice.h"
extern "C" void func_0015274c(struct AudioChannel *channel, float gain)
{
    channel->gain = gain;
    func_00152758(channel->state, gain);
}
