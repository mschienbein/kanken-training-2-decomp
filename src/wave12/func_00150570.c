#include "game/aux_effect.h"
#include "game/pool_list.h"







static __forceinline int is_ramping(const struct AuxFloatRamp *ramp)
{
    return ramp->elapsed < ramp->duration;
}

static __forceinline struct PoolLink *list_end(struct PoolList *list)
{
    return &list->sentinel;
}

static __forceinline void append_effect(struct PoolList *list, struct AuxEffectNode *effect)
{
    func_00191154(list, list_end(list), &effect->link);
}

int func_00150570(struct SoundSubsystem *manager, unsigned char channel,
                  struct AuxEffectNode *effect)
{
    struct PoolList *list;
    if (is_ramping(&manager->volume[channel]))
        func_0018362c(manager, channel);
    func_0017e654(&manager->volume[channel], 0, 1.0f);
    func_0017e848((signed char)channel, 1.0f);
    list = &manager->effects[channel];
    if (!list->count)
        func_00183614(channel, func_00152c88, (void *)(unsigned int)channel);
    append_effect(list, effect);
    return 1;
}
