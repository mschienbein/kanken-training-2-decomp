#include "game/audio_voice.h"

void func_00154624(struct AudioChannel *element, struct AudioVoiceGroup *group)
{
    group->channels[0] = 0;
    group->flag17 = 0;
    group->flag15 = 0;
    group->count = 0;
    func_0017e764(func_00185c58(), group);
    group->active = 0;
    if (group->callback)
        group->callback(group, 3, group->context);
}
