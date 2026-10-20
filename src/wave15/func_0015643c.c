#include "game/audio_voice.h"


void func_0015643c(struct AudioVoiceGroup *group)
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
