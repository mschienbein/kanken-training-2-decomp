#include "game/audio_voice.h"
#include "game/audio_dsp.h"
#include "game/service_runtime.h"

struct FilterValue {
    int b0, b1, b2, a1, a2;
    FilterValue(short b0_, short b1_, short b2_, short a1_, short a2_)
        : b0(b0_), b1(b1_), b2(b2_), a1(a1_), a2(a2_) {}
};

extern "C" int func_0014ce8c(ServiceCoordinator *coordinator, int channel,
    short b0, short b1, short b2, short a1, short a2)
{
    FilterValue coefficients(b0, b1, b2, a1, a2);
    func_00182210();
    if (func_0018f018()) {
        AudioDspParameterBlock *parameters =
            coordinator->storage.audio.auxiliary_parameters[coordinator->serial & 1][channel];
        short *values = parameters->settings.coefficients;
        values[0] = coefficients.a2;
        values[1] = coefficients.a1;
        values[2] = coefficients.b2;
        values[3] = coefficients.b1;
        values[4] = coefficients.b0;
        parameters->flags |= 0x1000000;
        func_00182204();
        return 1;
    }
    func_00182204();
    return 0;
}

