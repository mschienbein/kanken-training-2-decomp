#include "game/scene_lifecycle.h"
#include "game/audio_voice.h"



void func_00183550(struct AudioVoiceGroup *group)
{
    if (group->active) {
        for (int i = 0; i < group->count; ++i) {
            if (group->channels[i]) {
                func_001829d0(group->channels[i]);
                group->channels[i] = 0;
            }
        }
        group->count = 0;
        func_0017e764(func_00185c58(), group);
        group->active = 0;
    }
}
