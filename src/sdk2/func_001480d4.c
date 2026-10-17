#include "ctr/ipc.h"

struct IdentifierByteCommand {
    unsigned int header;
    union {
        unsigned short identifier;
        CtrResult result;
    } word;
    unsigned char output;
};

CtrResult func_001480d4(const CtrHandle *session, unsigned short identifier,
                        unsigned char *output)
{
    struct IdentifierByteCommand *command =
        (struct IdentifierByteCommand *)ctrIpcCommandBuffer();
    CtrResult result;

    command->header = ctrIpcHeader(2u, 1u, 0u);
    ctrIpcMarshalHalfword(&command->word.identifier, &identifier);
    result = svcSendSyncRequest(*session);
    if (ctrIpcTransportFailure(result) >= 0) {
        *output = command->output;
        result = command->word.result;
    }
    return result;
}
