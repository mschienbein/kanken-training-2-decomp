#include "ctr/ipc.h"

extern "C" {

CtrResult func_00148118(const CtrHandle *session, unsigned short identifier)
{
    struct IdentifierCommand *command =
        (struct IdentifierCommand *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command->header = 0x170040;
    ctrMarshalValue(command->word.identifier, identifier);
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = command->word.result;
    return result;
}

}
