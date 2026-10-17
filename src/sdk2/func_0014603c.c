#include "game/audio_voice.h"
#include "game/service_runtime.h"
void func_0014603c(struct HandleManager *owner)
{
    int i;
    for (i = 0; i < 24; ++i)
        func_0014e20c(owner->channels[i]->state);
}
