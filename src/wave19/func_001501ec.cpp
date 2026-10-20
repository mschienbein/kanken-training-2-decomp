#include "game/audio_voice.h"
#include "game/functions.h"

struct AudioChannel;

extern "C" void func_001501ec(nw::snd::internal::driver::Voice *group, float value)
{
    if (group->value28 != value) {
        group->value28 = value;
        group->dirty |= 4;
    }
}
