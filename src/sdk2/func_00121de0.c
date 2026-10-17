#include "ctr/ipc.h"

extern CtrHandle data_00243794;

unsigned func_00121de0(unsigned value)
{
    unsigned *command = (unsigned *)ctrIpcCommandBuffer();
    unsigned result;
    int failure;

    command[0] = ctrIpcHeader(20u, 1u, 0u);
    command[1] = value;
    result = (unsigned)svcSendSyncRequest(data_00243794);
    failure = (int)(result & 0x80000000u);
    if (failure >= 0)
        result = command[1];
    return result;
}
