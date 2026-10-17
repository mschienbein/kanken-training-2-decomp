#include "game/aux_effect.h"
static __forceinline float current_value(const struct AuxFloatRamp *ramp)
{
    if (ramp->elapsed >= ramp->duration)
        return ramp->target;
    return (ramp->target - ramp->start) * (float)ramp->elapsed / (float)ramp->duration + ramp->start;
}

void func_0017e654(struct AuxFloatRamp *ramp, int duration, float target)
{
    ramp->start = current_value(ramp);
    ramp->target = target;
    ramp->duration = duration;
    ramp->elapsed = 0;
}
