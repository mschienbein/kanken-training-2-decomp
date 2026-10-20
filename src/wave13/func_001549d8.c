#include "game/audio_dsp.h"
#include "game/audio_voice.h"
#include "game/service_runtime.h"
/* [INFERENCE] DSP buffer and parameter names; offsets follow the SDK accesses. */




int func_001549d8(struct ServiceCoordinator *context)
{
    unsigned int result;

    func_00182210();
    if (func_0018f018()) {
        result = context->replies[context->current_buffer]->value;
        func_00182204();
        return result;
    } else {
        func_00182204();
        return -1;
    }
}
