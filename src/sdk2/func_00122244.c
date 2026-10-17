#include "ctr/ipc.h"

extern CtrHandle data_00243144;

unsigned func_00122244(unsigned *output)
{
    unsigned *command = (unsigned *)ctrIpcCommandBuffer();
    unsigned result;
    int failure;

    command[0] = ctrIpcHeader(11u, 0u, 0u);
    result = (unsigned)svcSendSyncRequest(data_00243144);
    failure = (int)(result & 0x80000000u);
    if (failure >= 0) {
        *output = command[2];
        result = command[1];
    }
    return result;
}
