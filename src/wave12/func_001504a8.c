#include "game/aux_effect.h"
#include "game/pool_list.h"







static __forceinline void prepare_channel(struct SoundSubsystem *manager, unsigned char channel)
{
    if (manager->volume[channel].elapsed < manager->volume[channel].duration)
        func_0018362c(manager, channel);
    func_0017e654(&manager->volume[channel], 0, 1.0f);
    func_0017e848((signed char)channel, 1.0f);
}

int func_001504a8(struct SoundSubsystem *manager, unsigned char channel,
                  struct AuxDelayProcessor *effect)
{
    prepare_channel(manager, channel);
    return func_001525c8(channel, effect);
}
