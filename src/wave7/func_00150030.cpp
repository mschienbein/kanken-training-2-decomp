#include "game/audio_voice.h"
extern "C" void func_00150030(nw::snd::internal::driver::Voice *group, float value)
{
    if (group->value38 != value) {
        group->value38 = value;
        group->dirty |= 0x10;
    }
}
