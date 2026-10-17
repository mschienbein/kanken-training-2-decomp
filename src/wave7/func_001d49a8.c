#include "game/audio_voice.h"
void func_001d49a8(struct AudioVoiceGroup *group, unsigned char value)
{
    if (group->option2c != value) {
        group->option2c = value;
        group->dirty |= 8;
    }
}
