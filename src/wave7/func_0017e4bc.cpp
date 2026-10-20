#include "game/audio_voice.h"

extern "C" void func_0017e4bc(nw::snd::internal::driver::Voice *group, int mode)
{
    int i;
    for (i = 0; i < group->count; ++i) {
        if (group->channels[i] != 0)
            func_001545ec(group->channels[i], mode);
    }
    group->mode23 = mode;
}
