#include "ctr/ipc.h"


unsigned func_00121d28(unsigned *first, unsigned *second, unsigned *third,
                       unsigned *fourth, unsigned *fifth, unsigned *sixth)
{
    unsigned *out0 = first;
    unsigned *out1 = second;
    unsigned *out2 = third;
    unsigned *out3 = fourth;
    unsigned *out4 = fifth;
    unsigned *out5 = sixth;
    unsigned *command = ctrIpcCommandBuffer();
    unsigned result;
    int failure;

    command[0] = ctrIpcHeader(10u, 0u, 0u);
    result = (unsigned)svcSendSyncRequest(data_0024311c.handle);
    failure = (int)(result & 0x80000000u);
    if (failure >= 0) {
        *out0 = command[3];
        *out1 = command[4];
        *out2 = command[5];
        *out3 = command[6];
        *out4 = command[7];
        *out5 = command[8];
        result = command[1];
    }
    return result;
}
