#include "game/service_state.h"
#include <string.h>
#include "ctr/cp15.h"
#include "ctr/svc.h"


CtrResult func_0013e770(const CtrHandle *session, void *buffer, unsigned int size,
                       unsigned short first, unsigned short second,
                       signed char *output)
{
    struct X07NarrowCommand *const command =
        (struct X07NarrowCommand *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command->header = 0x1100c2;
    command->body.size = size;
    memcpy(&command->first.first, &first, sizeof(first));
    memcpy(&command->second, &second, sizeof(second));
    command->descriptor = 0xau;
    command->descriptor |= size << 4;
    command->buffer = buffer;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0) {
        *output = command->first.output;
        result = command->body.result;
    }
    return result;
}
