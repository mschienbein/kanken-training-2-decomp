#include "game/audio_voice.h"

extern "C" void func_0017e6a4(nw::snd::internal::driver::Voice *group, float value)
{
    if (group->value30 == value)
        return;
    group->value30 = value;
    group->dirty |= 8;
}
