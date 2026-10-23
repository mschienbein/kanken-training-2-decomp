#include "game/service_runtime.h"
#include "game/audio_dsp.h"

extern "C" int func_00148a9c(struct ServiceCoordinator *state, unsigned short value)
{
    func_00182210();
    if (!func_0018f018() || (value != 0 && value != 1)) {
        func_00182204();
        return 0;
    }
    struct AudioDspParameterBlock *request =
        state->storage.audio.voice_parameters[state->completion_slot];
    request->settings.value1e = value;
    request->flags |= 0x40000000u;
    func_00182204();
    return 1;
}
