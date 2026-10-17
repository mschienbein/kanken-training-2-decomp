#include "ctr/ipc.h"

struct IdentifierShortCommand {
    unsigned int header;
    union {
        unsigned short identifier;
        CtrResult result;
    } word;
    unsigned short output;
};

CtrResult func_001481cc(const CtrHandle *session, unsigned short identifier,
                        unsigned short *output)
{
    struct IdentifierShortCommand *command =
        (struct IdentifierShortCommand *)ctrIpcCommandBuffer();
    CtrResult result;

    command->header = ctrIpcHeader(1u, 1u, 0u);
    ctrIpcMarshalHalfword(&command->word.identifier, &identifier);
    result = svcSendSyncRequest(*session);
    if (ctrIpcTransportFailure(result) >= 0) {
        *output = command->output;
        result = command->word.result;
    }
    return result;
}
