#include "game/service_runtime.h"

#include "game/audio_dsp.h"

int func_0014ce14(struct ServiceCoordinator *coordinator, int channel,
    short first, short second)
{
    struct AudioDspParameterBlock *parameters;
    func_00182210();
    if (func_0018f018()) {
        parameters = coordinator->storage.audio.auxiliary_parameters[coordinator->serial & 1][channel];
        parameters->first = first;
        parameters->second = second;
        parameters->flags |= 0x800000;
        func_00182204();
        return 1;
    }
    func_00182204();
    return 0;
}
