#include "game/audio_voice.h"

void func_001d4aac(struct AudioVoiceGroup *group)
{
    group->flag15 = 1;
    group->flag17 = 0;
    group->dirty |= 1;
}
