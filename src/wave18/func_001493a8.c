#include "game/functions.h"
/* [INFERENCE] Step interpolation uses the signed representation ordering of
 * IEEE-754 inputs, rather than a floating-point comparison. */
float func_001493a8(float value)
{
    union {
        float value;
        int representation;
    } input;
    input.value = value;
    return input.representation < 0x3f000000 ? 0.0f : 1.0f;
}
