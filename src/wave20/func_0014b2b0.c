#include "game/matrix.h"

float func_0014b2b0(unsigned angle)
{
    float fraction = (float)(angle & 255u) * (1.0f / 256.0f);
    const struct TrigEntry *entry = &data_0021b100[angle >> 8];
    return entry->sine + entry->sine_step * fraction;
}
