#include "game/audio_voice.h"
#include "game/pool_list.h"



void func_0014ee5c(struct AudioVoiceGroup *group)
{
    if (group->active && group->flag16 && group->channels[0] && group->record &&
        group->record->flags.bytes.second != 1 && group->record->flags.bytes.second != 2) {
        if (group->callback)
            group->callback(group, 0, group->context);
        group->flag16 = 0;
        group->flag15 = 0;
    }
}
