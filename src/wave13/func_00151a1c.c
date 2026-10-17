#include "game/audio_voice.h"
float func_00151a1c(const struct AudioLinearRamp *ramp)
{
    if (ramp->elapsed < ramp->duration)
        return ramp->start + (ramp->end - ramp->start) * (float)ramp->elapsed / (float)ramp->duration;
    return ramp->end;
}
