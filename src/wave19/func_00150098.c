#include "game/audio_voice.h"

void func_00150098(struct AudioVoiceGroup *group, float value)
{
    if (group->value34 != value) {
        group->value34 = value;
        group->dirty |= 8;
    }
}
