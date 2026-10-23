#include "game/service_runtime.h"


#include "game/audio_dsp.h"

static __forceinline void set_volume(struct AudioDspVolumeSettings *settings,
    int index, float value)
{
    /* ORIGINAL BUG: this two-output array is indexed without validation. */
    settings->output_volume[index] = value;
}

void func_00180d9c(struct ServiceCoordinator *coordinator, int index, float value)
{
    func_00182210();
    if (func_0018f018()) {
        struct AudioDspParameterBlock *parameters =
            coordinator->storage.audio.voice_parameters[coordinator->completion_slot];
        set_volume(&parameters->volume, index, value);
        if (index == 0)
            parameters->flags |= 0x1000000;
        else if (index == 1)
            parameters->flags |= 0x2000000;
    }
    func_00182204();
}
