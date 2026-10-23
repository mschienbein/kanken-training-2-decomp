#include "game/service_runtime.h"

#include "game/runtime.h"
#include "game/audio_voice.h"

static int submit_coefficients(struct ServiceCommandQueueW22E *queue, int channel,
                               const struct AdpcmCoefficients *source)
{
    func_00182210();
    if (func_0018f018()) {
        unsigned command_buffer = queue->command_buffer & 1;
        unsigned destination_buffer = queue->destination_buffer;
        struct ServiceCommandW22E *command = queue->commands[command_buffer][channel];
        __aeabi_memcpy4(queue->destinations[destination_buffer][channel], source,
                       sizeof(*source));
        command->flags |= 4;
        func_00182204();
        return 1;
    }
    func_00182204();
    return 0;
}

int func_0014cd18(struct ServiceCommandQueueW22E *queue, int channel,
                  const struct AdpcmCoefficients *source)
{
    return submit_coefficients(queue, channel, source);
}
