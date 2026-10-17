#include "game/runtime.h"
#include "game/service_state.h"
#include "ctr/cp15.h"
#include "ctr/svc.h"


static __inline void copy_payload(unsigned *destination, const void *source)
{
    func_00192720(destination, source, 128);
}

static __inline int transport_failure(CtrResult result)
{
    return (int)((unsigned)result & 0x80000000u);
}

CtrResult func_0012ec54(const CtrHandle *session, const void *payload)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;

    command[0] = (1u << 16) | (32u << 6);
    copy_payload(&command[1], payload);
    result = svcSendSyncRequest(*session);
    if (transport_failure(result) >= 0)
        result = (CtrResult)command[1];
    return result;
}
