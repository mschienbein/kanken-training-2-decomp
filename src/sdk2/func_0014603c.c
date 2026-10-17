#include "game/audio_voice.h"
void func_0014603c(struct AudioChannelOwner *owner)
{
    int i;
    for (i = 0; i < 24; ++i)
        func_0014e20c(owner->channels[i]->state);
}
