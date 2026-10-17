#include "game/service_state.h"
#include "ctr/cp15.h"
#include "ctr/svc.h"
#include <string.h>

CtrResult func_0012fa24(const CtrHandle *session, unsigned int register_address,
                        const void *source, unsigned int size)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    unsigned int descriptor = 0;
    CtrResult result;
    int failure;

    command[0] = (1u << 16) | (2u << 6) | 2u;
    memcpy(&command[1], &register_address, sizeof(register_address));
    memcpy(&command[2], &size, sizeof(size));
    descriptor |= size << 14;
    command[3] = descriptor | 2u;
    memcpy(&command[4], &source, sizeof(source));
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
