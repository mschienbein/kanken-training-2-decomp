#include "ctr/ipc.h"

extern CtrHandle data_00243794;

unsigned int func_0013e7d4(void)
{
    unsigned int *command = (unsigned int *)ctrIpcCommandBuffer();
    unsigned int result;
    int failure;

    command[0] = ctrIpcHeader(2u, 0u, 2u);
    command[1] = 0x20;
    result = (unsigned int)svcSendSyncRequest(data_00243794);
    failure = (int)(result & 0x80000000u);
    if (failure >= 0)
        result = command[1];
    return result;
}
