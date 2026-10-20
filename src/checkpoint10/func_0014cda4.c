#include "game/service_runtime.h"

#include "game/audio_dsp.h"

int func_0014cda4(struct ServiceCoordinator *coordinator, int channel, short value)
{
    struct AudioDspParameterBlock *parameters;
    func_00182210();
    if (func_0018f018()) {
        parameters = coordinator->storage.audio.auxiliary_parameters[coordinator->serial & 1][channel];
        parameters->value3a = value;
        parameters->flags |= 0x400000;
        func_00182204();
        return 1;
    }
    func_00182204();
    return 0;
}
