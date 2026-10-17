#include "game/audio_voice.h"
extern void func_0014e218(struct AudioChannelState *state);

void func_0014e20c(struct AudioChannelState *state)
{
    state->flags = 0xffff;
    func_0014e218(state);
}
