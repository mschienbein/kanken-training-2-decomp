#include "game/animation.h"

void func_0019f450(float rate)
{
    bss_00495b30.rate = rate;
    bss_00495b30.milliseconds = 1000.0f / rate;
    bss_00495b30.nominalFrames = 60.0f / rate;
}
