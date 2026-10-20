#include "game/audio_voice.h"
extern "C" void func_001d49c8(nw::snd::internal::driver::Voice *group, unsigned char value)
{
    if (group->option2d != value) {
        group->option2d = value;
        group->dirty |= 8;
    }
}
