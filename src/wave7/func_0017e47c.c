#include "game/audio_voice.h"

void func_0017e47c(struct AudioVoiceGroup *group, unsigned int value)
{
    int index;
    for (index = 0; index < group->count; ++index) {
        if (group->channels[index])
            func_00154608(group->channels[index], value);
    }
}
