#include "game/layout_draw.h"
#include "game/audio_voice.h"


void func_001c5978(struct AudioChannelState *state, unsigned int value)
{
    struct ChannelFlagWord *flags = (struct ChannelFlagWord *)&state->flags1e;
    flags->mode = value;
}
