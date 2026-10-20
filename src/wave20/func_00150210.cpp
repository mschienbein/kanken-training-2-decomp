#include "game/audio_voice.h"

extern "C" void func_00150210(nw::snd::internal::driver::Voice *group, int index, float value)
{
    if (value < 0.0f)
        value = 0.0f;
    if (group->sends[index] != value) {
        group->sends[index] = value;
        group->dirty |= 8;
    }
}
