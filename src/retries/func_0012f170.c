#include "game/service_configuration_words.h"
#include "game/service_protocol.h"
#include "game/runtime.h"
#include "ctr/cp15.h"
#include "ctr/svc.h"



CtrResult func_0012f170(struct ServiceConfigurationWords *out)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 24u << 16;
    result = svcSendSyncRequest(data_002430d8.handle);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure < 0)
        return result;
    {
        const unsigned int *response = &command[2];
        __aeabi_memcpy4(out, response, sizeof *out);
    }
    return (CtrResult)command[1];
}
