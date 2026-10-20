#include "game/audio_dsp.h"
#include "game/audio_voice.h"
#include "game/service_runtime.h"
/* [INFERENCE] DSP buffer and parameter names; offsets follow the SDK accesses. */




int func_00148854(struct ServiceCoordinator *context, unsigned int value)
{
    struct VoiceDspParameters *parameters;

    func_00182210();
    if (func_0018f018()) {
        if (value > 0x8000u)
            value = 0x8000u;
        parameters =
            context->voice[context->completion_slot];
        parameters->parameters.value22 = value;
        parameters->flags |= 0x80000000u;
        func_00182204();
        return 1;
    } else {
        func_00182204();
        return 0;
    }
}
