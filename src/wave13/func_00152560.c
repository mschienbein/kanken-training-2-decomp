#include "game/audio_dsp.h"
#include "game/audio_voice.h"
#include "game/service_runtime.h"
/* [INFERENCE] DSP buffer and parameter names; offsets follow the SDK accesses. */




int func_00152560(struct ServiceCoordinator *context, unsigned int channel)
{
    struct AudioDspParameterBlock *parameters;

    func_00182210();
    if (func_0018f018()) {
        parameters = context->storage.audio.auxiliary_parameters[context->serial & 1][channel];
        parameters->flags |= 0x20000000u;
        func_00182204();
        return 1;
    } else {
        func_00182204();
        return 0;
    }
}
