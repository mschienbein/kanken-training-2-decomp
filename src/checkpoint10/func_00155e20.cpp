#include "game/service_runtime.h"
#include "game/audio_dsp.h"

extern "C" int func_00155e20(struct ServiceCoordinator *state, int channel)
{
    func_00182210();
    if (func_0018f018()) {
        state->storage.audio.auxiliary_parameters[state->serial & 1][channel]->flags |= 16;
        func_00182204();
        return 1;
    }
    func_00182204();
    return 0;
}
