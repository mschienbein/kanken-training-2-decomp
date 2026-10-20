#include "ctr/ipc.h"

union ServiceWord {
    unsigned int word;
    unsigned char byte;
};

struct ServiceIpcPacket {
    unsigned int header;
    union ServiceWord words[4];
};



CtrResult func_001380c8(unsigned int value0, unsigned int value1,
                         unsigned int value2, unsigned int value3, unsigned char *out)
{
    struct ServiceIpcPacket *command =
        (struct ServiceIpcPacket *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(15u, 4u, 0u);
    memcpy(&command->words[0].byte, &value0, 1);
    command->words[1].word = value1;
    memcpy(&command->words[2].byte, &value2, 1);
    command->words[3].word = value3;
    result = svcSendSyncRequest(data_00243040.handle);
    failure = ctrIpcTransportFailure(result);
    if (failure < 0)
        return result;
    *out = command->words[1].byte;
    return (CtrResult)command->words[0].word;
}
