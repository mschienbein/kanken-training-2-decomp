#include "game/audio_voice.h"
void func_001d49c8(struct AudioVoiceGroup *group, unsigned char value)
{
    if (group->option2d != value) {
        group->option2d = value;
        group->dirty |= 8;
    }
}
