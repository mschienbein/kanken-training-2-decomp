#include "ctr/ipc.h"

struct ServiceStatusCommand {
    unsigned int header;
    union { unsigned char selector; CtrResult result; } status;
    unsigned char enabled;
    unsigned char padding[3];
    unsigned int values[3];
};

CtrResult func_00123538(unsigned char selector, unsigned char *enabled,
                        unsigned int *first, unsigned int *second,
                        unsigned int *third)
{
    unsigned char *enabled_out = enabled;
    unsigned *first_out = first, *second_out = second, *third_out = third;
    struct ServiceStatusCommand *command =
        (struct ServiceStatusCommand *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(5u, 1u, 0u);
    ctrIpcMarshalByte(&command->status.selector, &selector);
    result = svcSendSyncRequest(data_00243040.handle);
    failure = ctrIpcTransportFailure(result);
    if (failure < 0)
        return result;
    *enabled_out = command->enabled;
    *first_out = command->values[0];
    *second_out = command->values[1];
    *third_out = command->values[2];
    return command->status.result;
}
