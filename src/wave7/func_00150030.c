#include "game/audio_voice.h"
void func_00150030(struct AudioVoiceGroup *group, float value)
{
    if (group->value38 != value) {
        group->value38 = value;
        group->dirty |= 0x10;
    }
}
