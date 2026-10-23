#include "game/service_runtime.h"


int func_0014cc9c(struct ServiceCommandQueueW22E *queue, int channel, float value)
{
    struct ServiceCommandW22E *command;
    func_00182210();
    if (func_0018f018()) {
        command = queue->commands[queue->command_buffer & 1][channel];
        command->value = value;
        command->flags |= 0x40000;
        func_00182204();
        return 1;
    }
    func_00182204();
    return 0;
}
