#include "ctr/ipc.h"



unsigned func_00121da4(unsigned value)
{
    unsigned *command = (unsigned *)ctrIpcCommandBuffer();
    unsigned result;
    int failure;

    command[0] = ctrIpcHeader(6u, 1u, 0u);
    command[1] = value;
    result = (unsigned)svcSendSyncRequest(data_00243794.handle);
    failure = (int)(result & 0x80000000u);
    if (failure >= 0)
        result = command[1];
    return result;
}
