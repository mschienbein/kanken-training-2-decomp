#include "game/audio_voice.h"
#include "game/pool_list.h"


unsigned char func_001f2f2c(const struct AudioVoiceGroup *group)
{
    return group->channels[0] != 0 ? group->format : 1;
}
