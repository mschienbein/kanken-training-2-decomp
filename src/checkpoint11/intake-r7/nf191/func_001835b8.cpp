#include "game/audio_voice.h"
static inline void disable_channels(struct AudioVoiceGroup *group)
{
    for (int index = 0; index < group->count; ++index)
        if (group->channels[index] != 0)
            func_001526b4(group->channels[index], 1);
}
void func_001835b8(struct AudioVoiceGroup *group)
{
    if (group->flag16) {
        disable_channels(group);
        group->flag16 = 0;
    }
    group->flag18 = 0;
    group->flag17 = 0;
    group->flag15 = 0;
}
