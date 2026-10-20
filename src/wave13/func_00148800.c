#include "game/audio_dsp.h"
#include "game/audio_voice.h"
#include "game/service_runtime.h"
/* [INFERENCE] DSP buffer and parameter names; offsets follow the SDK accesses. */




void func_00148800(struct ServiceCoordinator *context, unsigned int value)
{
    struct VoiceDspParameters *parameters;

    func_00182210();
    if (func_0018f018()) {
        parameters =
            context->voice[context->completion_slot];
        parameters->valuebc = value;
        parameters->flagsc0 |= 0x00010000u;
        func_00182204();
    } else {
        func_00182204();
    }
}
