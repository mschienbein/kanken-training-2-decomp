#include "game/audio_voice.h"

extern "C" void func_0014fbcc(struct AudioChannel *owner, int enabled)
{
    if (enabled)
        owner->flags |= 2;
    else
        owner->flags &= ~2;
    func_00152610(owner->state, owner->flags);
}
