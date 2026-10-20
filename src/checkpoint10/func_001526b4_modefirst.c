#include "game/audio_voice.h"
#include "game/stream_manager.h"



void func_001526b4(struct AudioChannel *channel, int mode)
{
    int next_mode = mode;
    channel->mode = next_mode;
    if (next_mode == 1) {
        struct AudioChannelState *state = channel->state;
        func_0014fc98(state);
    }
    func_001526e0(channel->state, next_mode);
}
