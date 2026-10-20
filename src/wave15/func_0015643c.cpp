#include "game/audio_voice.h"

extern void func_00155dec(struct AudioChannel *channel, int priority);

extern "C" void func_0015643c(nw::snd::internal::driver::Voice *group)
{
    int index;
    if (group->priority != 1) {
        for (index = 0; index < group->count; ++index) {
            struct AudioChannel *channel = group->channels[index];
            if (channel)
                func_00155dec(channel, (int)group->priority);
        }
    }
}
