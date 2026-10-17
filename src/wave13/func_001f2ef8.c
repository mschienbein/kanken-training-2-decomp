#include "game/audio_voice.h"


/* The caller adds this sample position to its stream base and wraps it at a loop boundary. */
unsigned func_001f2ef8(const struct AudioVoiceGroup *group)
{
    if (group->channels[0] != 0)
        return func_001efcd0(group->channels[0]);
    return 0;
}
