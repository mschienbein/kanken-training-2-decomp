#include "game/sequence.h"
#include "game/audio_voice.h"

extern void func_00155d68(struct TaskPool *pool, struct AudioVoiceGroup *group);

void func_00155148(struct AudioVoiceGroup *group, int priority)
{
    int index;
    group->priority = priority;
    func_00155d68(func_00185c58(), group);
    if (group->priority == 1) {
        for (index = 0; index < group->count; ++index) {
            struct AudioChannel *channel = group->channels[index];
            if (channel)
                func_00155dec(channel, 1);
        }
    }
}
