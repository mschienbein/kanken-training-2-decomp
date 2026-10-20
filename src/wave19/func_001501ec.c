#include "game/audio_voice.h"
#include "game/functions.h"

struct AudioChannel;

void func_001501ec(struct AudioVoiceGroup *group, float value)
{
    if (group->value28 != value) {
        group->value28 = value;
        group->dirty |= 4;
    }
}
