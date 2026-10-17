#include "game/audio_voice.h"

void func_001525f0(struct AudioChannel *owner, int enabled)
{
    if (enabled)
        owner->flags |= 1;
    else
        owner->flags &= ~1;
    func_00152610(owner->state, owner->flags);
}
