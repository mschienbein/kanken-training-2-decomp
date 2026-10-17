#include "game/command_group.h"
extern "C" int func_001f1890(const OptionalVoiceMode *voice)
{
    if (!voice->active)
        return 0;
    return voice->mode;
}
