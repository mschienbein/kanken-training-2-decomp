#include "game/command_group.h"
#include "game/audio_voice.h"

void func_001c5958(struct AudioChannelState *state, unsigned int mode)
{
    state->flags1e = (state->flags1e & ~3u) | (mode & 3u);
}
