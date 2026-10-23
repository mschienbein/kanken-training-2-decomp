#include "game/service_runtime.h"
#include "game/audio_dsp.h"

static void set_mode(struct AudioDspSettings *settings, int mode,
                     unsigned char option)
{
    settings->mode38 = mode;
    if (mode == 0)
        settings->option39 = option;
}

int func_0014cc1c(struct ServiceCoordinator *coordinator, int channel,
                  int mode, unsigned char option)
{
    int selected_channel;
    func_00182210();
    selected_channel = channel;
    if (func_0018f018()) {
        struct AudioDspParameterBlock *parameters =
            coordinator->storage.audio.
            auxiliary_parameters[coordinator->serial & 1][selected_channel];
        set_mode(&parameters->settings, mode, option);
        parameters->flags |= 0x20000;
        func_00182204();
        return 1;
    }
    func_00182204();
    return 0;
}
