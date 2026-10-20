#include "ctr/ipc.h"



CtrResult func_00115998(unsigned int application_id, unsigned int flags,
                        CtrHandle *notification_event, CtrHandle *pause_event)
{
    unsigned int *command = (unsigned int *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0] = ctrIpcHeader(2u, 2u, 0u);
    command[1] = application_id;
    command[2] = flags;
    result = svcSendSyncRequest(data_00243040.handle);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *notification_event = command[3];
        *pause_event = command[4];
        result = (CtrResult)command[1];
    }
    return result;
}
