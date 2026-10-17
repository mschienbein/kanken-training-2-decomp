#include "game/audio_dsp.h"
#include "game/audio_voice.h"
#include "game/service_runtime.h"
/* [INFERENCE] DSP buffer and parameter names; offsets follow the SDK accesses. */




void func_001489e8(struct ServiceCoordinator *context, unsigned int value)
{
    struct AudioDspParameterBlock *parameters;

    func_00182210();
    if (func_0018f018()) {
        parameters =
            context->storage.audio.voice_parameters[context->completion_slot];
        parameters->value10 = value;
        parameters->flags |= 0x00008000u;
        func_00182204();
    } else {
        func_00182204();
    }
}
