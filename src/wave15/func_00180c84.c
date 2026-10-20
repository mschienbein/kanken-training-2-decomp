#include "game/service_runtime.h"
#include "game/audio_dsp.h"

static __forceinline void set_route(struct VoiceDspPayload *parameters,
                                    int channel, short value)
{
    parameters->route[channel] = value;
}

int func_00180c84(struct ServiceCoordinator *driver, int channel, short value)
{
    struct VoiceDspParameters *command;
    func_00182210();
    if (!func_0018f018()) {
        func_00182204();
        return 0;
    }
    command = driver->voice[driver->completion_slot];
    /* ORIGINAL BUG: the channel is used for the write before the switch. */
    set_route(&command->parameters, channel, value);
    switch (channel) {
    case 0:
        command->flags |= 0x40;
        break;
    case 1:
        command->flags |= 0x80;
        break;
    }
    func_00182204();
    return 1;
}
