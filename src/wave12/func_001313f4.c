#include "game/audio_tasks.h"
#include "game/aux_effect.h"
#include "game/pool_list.h"







void func_001313f4(struct SoundSubsystem *manager)
{
    if (manager->master_volume.active) {
        int channel;
        for (channel = 0; channel < 2; channel++) {
            func_0018362c(manager, channel);
            func_00183614(channel, manager->saved_callbacks[channel], manager->saved_contexts[channel]);
            manager->saved_callbacks[channel] = 0;
            manager->saved_contexts[channel] = 0;
        }
        manager->master_volume.active = 0;
    }
}
