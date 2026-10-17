#include "game/audio_voice.h"
void func_0014fc78(struct AudioChannel *channel, float volume)
{
    channel->volume = volume >= 0.0f ? volume : 0.0f;
    func_0015268c(channel->state, volume);
}
