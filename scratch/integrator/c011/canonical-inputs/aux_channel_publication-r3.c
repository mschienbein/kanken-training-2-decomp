#include "game/aux_channel_parameters.h"

static struct AudioDspParameterBlock *channel_parameters(
    struct ServiceCoordinator *coordinator, int channel)
{
    return coordinator->storage.audio.auxiliary_parameters[coordinator->serial & 1][channel];
}

static int publish_channel_parameters(struct AudioDspParameterBlock *parameters,
    unsigned int changed)
{
    parameters->flags |= changed;
    func_00182204();
    return 1;
}

int func_00155a44(struct ServiceCoordinator *coordinator, int channel)
{
    func_00182210();
    if (func_0018f018()) {
        struct AudioDspParameterBlock *parameters = channel_parameters(coordinator, channel);
        parameters->auxiliary_enabled = 1;
        return publish_channel_parameters(parameters, 0x10000);
    }
    func_00182204();
    return 0;
}

/* Context reproduction only: candidate for rehoming, not new credit. */
int func_00155ab4(struct ServiceCoordinator *coordinator, int channel, short count)
{
    func_00182210();
    if (func_0018f018()) {
        struct AudioDspParameterBlock *parameters = channel_parameters(coordinator, channel);
        parameters->auxiliary_count = count;
        return publish_channel_parameters(parameters, 0x10000000);
    }
    func_00182204();
    return 0;
}
