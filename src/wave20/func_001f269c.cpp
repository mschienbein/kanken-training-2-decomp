#include "game/biquad_filter.h"

/* The NW4C BiquadFilterCallback interface preserves the unused type argument. */

extern "C" void func_001f269c(const BiquadFilterCallback *callback, int type,
                             float value, BiquadCoefficients *coefficients)
{
    int index = (int)((2.0f - value) * value * 92.0f);
    if (index > 92)
        index = 92;
    else if (index < 0)
        index = 0;
    *coefficients = data_0021db70[index];
}
