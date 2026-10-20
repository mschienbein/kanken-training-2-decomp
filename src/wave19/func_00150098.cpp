#include "game/audio_voice.h"

extern "C" void func_00150098(nw::snd::internal::driver::Voice *group, float value)
{
    if (group->value34 != value) {
        group->value34 = value;
        group->dirty |= 8;
    }
}
