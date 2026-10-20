#include "game/audio_voice.h"

extern "C" void func_001d4aac(nw::snd::internal::driver::Voice *group)
{
    group->flag15 = 1;
    group->flag17 = 0;
    group->dirty |= 1;
}
