#include "game/audio_voice.h"
#include "game/service_runtime.h"
#include "game/stream_manager.h"




/* [INFERENCE] DSP buffer and parameter names; offsets follow the SDK accesses. */





void func_001526e0(struct AudioChannelState *state, int mode)
{
    state->mode = mode;
    switch (mode) {
    case 0:
        break;
    case 1:
        func_001524ec(&bss_004b5284, state->channel & 0xffu);
        state->active = 0;
        func_00152560(&bss_004b5284, state->channel & 0xffu);
        break;
    case 2:
        func_001524ec(&bss_004b5284, state->channel & 0xffu);
        break;
    default:
        break;
    }
}
