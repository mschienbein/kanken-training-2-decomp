#include "game/audio_voice.h"

void func_0017e6a4(struct AudioVoiceGroup *group, float value)
{
    if (group->value30 == value)
        return;
    group->value30 = value;
    group->dirty |= 8;
}
