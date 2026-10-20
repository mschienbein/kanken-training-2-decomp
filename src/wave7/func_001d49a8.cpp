#include "game/audio_voice.h"
extern "C" void func_001d49a8(nw::snd::internal::driver::Voice *group, unsigned char value)
{
    if (group->option2c != value) {
        group->option2c = value;
        group->dirty |= 8;
    }
}
