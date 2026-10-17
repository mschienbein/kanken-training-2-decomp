#include "game/sound_track.h"
/* [INFERENCE] The sole direct caller supplies an enable flag and ignores the result. */
void func_0018eef0(int enabled)
{
    if (enabled)
        func_00130308();
    else
        func_0018ef04();
}
