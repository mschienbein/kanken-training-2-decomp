#include "game/service_runtime.h"

struct ServiceCommandW22E {
    unsigned flags;
    unsigned char reserved04[0x34 - 4];
    float value;
    unsigned char reserved38[0xb0 - 0x38];
    unsigned parameter;
    unsigned char reservedb4[8];
    unsigned short options, identifier;
};
struct ServiceCommandDataW22E { unsigned words[8]; };
struct ServiceCommandDescriptorW22E {
    unsigned char reserved00[8];
    unsigned short identifier;
};
struct ServiceCommandQueueW22E {
    unsigned char reserved00[0x10b4];
    struct ServiceCommandW22E *commands[2][24];
    struct ServiceCommandDescriptorW22E *descriptors[2][24];
    struct ServiceCommandDataW22E *destinations[2][24];
    unsigned char reserved12f4[0x131e - 0x12f4];
    unsigned short command_buffer;
    unsigned short descriptor_buffer;
    unsigned short destination_buffer;
};

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
