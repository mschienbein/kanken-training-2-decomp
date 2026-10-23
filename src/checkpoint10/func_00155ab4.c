#include "game/service_runtime.h"

#include "game/audio_dsp.h"

int func_00155ab4(struct ServiceCoordinator *coordinator, int channel, short count)
{
    struct AudioDspParameterBlock *parameters;
    func_00182210();
    if (func_0018f018()) {
        parameters = coordinator->storage.audio.auxiliary_parameters[coordinator->serial & 1][channel];
        parameters->count = count;
        parameters->flags |= 0x10000000;
        func_00182204();
        return 1;
    }
    func_00182204();
    return 0;
}
