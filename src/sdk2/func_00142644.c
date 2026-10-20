#include "ctr/ipc.h"



unsigned int func_00142644(unsigned int value)
{
    unsigned int *command = (unsigned int *)ctrIpcCommandBuffer();
    unsigned int result;
    int failure;

    command[0] = ctrIpcHeader(1u, 1u, 2u);
    command[1] = value;
    command[2] = 0x20;
    result = (unsigned int)svcSendSyncRequest(data_00243794.handle);
    failure = (int)(result & 0x80000000u);
    if (failure >= 0)
        result = command[1];
    return result;
}
