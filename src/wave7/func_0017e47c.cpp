#include "game/audio_voice.h"

extern "C" void func_0017e47c(nw::snd::internal::driver::Voice *group, unsigned int value)
{
    int index;
    for (index = 0; index < group->count; ++index) {
        if (group->channels[index])
            func_00154608(group->channels[index], value);
    }
}
