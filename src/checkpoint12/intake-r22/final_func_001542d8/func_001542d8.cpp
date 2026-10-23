#include "game/service_runtime.h"
#include "game/exp_heap.h"

#include "game/aux_effect.h"

static void process_block(int channel, ServiceOutput *owner, int *samples)
{
    StreamLockGuard guard(&owner->mutex);
    int *plane1 = samples + 160;
    int *plane2 = samples + 320;
    int *plane3 = samples + 480;
    int *input[4] = {samples, plane1, plane2, plane3};
    if (owner->workspaces[channel].channels)
        func_00154a28(owner->workspaces[channel].channels, input);
    else if (owner->workspaces[channel].extended)
        func_00154b18(owner->workspaces[channel].extended, input);
}

extern "C" void func_001542d8(ServiceOutput *owner, int channel, int *samples)
{
    process_block(channel, owner, samples);
}
