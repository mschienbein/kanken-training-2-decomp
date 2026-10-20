/* SPDX-License-Identifier: GPL-3.0-or-later
 * Step-3 reconstruction. Address adjacency is only an [INFERENCE] of TU
 * membership; this subsystem split does not claim recovered historical files.
 */
#include "game/service_runtime.h"
#include "game/audio_dsp.h"
#include "game/audio_voice.h"

static AudioDspParameterBlock *auxiliary_parameters(ServiceCoordinator *, unsigned);
static void update_parameters(AudioDspParameterBlock *, unsigned, unsigned);

int func_001524ec(ServiceCoordinator *context, unsigned channel)
{
    func_00182210();
    if (!func_0018f018()) {
        func_00182204();
        return 0;
    }
    AudioDspParameterBlock *parameters = auxiliary_parameters(context, channel);
    update_parameters(parameters, 0xffu, 0x10000u);
    func_00182204();
    return 1;
}

int func_00152560(ServiceCoordinator *context, unsigned channel)
{
    func_00182210();
    if (func_0018f018()) {
        AudioDspParameterBlock *parameters = auxiliary_parameters(context, channel);
        update_parameters(parameters, 0, 0x20000000u);
        func_00182204();
        return 1;
    } else {
        func_00182204();
        return 0;
    }
}

static AudioDspParameterBlock *auxiliary_parameters(ServiceCoordinator *context, unsigned channel)
{
    return context->storage.audio.auxiliary_parameters[context->serial & 1][channel];
}

static void update_parameters(AudioDspParameterBlock *parameters, unsigned clear_control, unsigned flags)
{
    unsigned short &control = parameters->auxiliary_control.value;
    if (clear_control)
        control &= ~clear_control;
    parameters->flags |= flags;
}
