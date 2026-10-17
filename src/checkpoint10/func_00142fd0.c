#include "ctr/ipc.h"

/* [INFERENCE] Role names: word 1 is the service's first input parameter,
 * word 2 the byte count that also sizes the static input buffer, word 4 the
 * copied handle. The command id (0x1e) and word counts come from the header. */
struct IpcStaticInput_00142fd0 {
    unsigned int descriptor;
    const void *buffer;
};

struct IpcRequest_00142fd0 {
    unsigned int header;
    unsigned int parameter;
    unsigned int size;
    unsigned int handle_descriptor;
    CtrHandle handle;
    struct IpcStaticInput_00142fd0 input;
};

CtrResult func_00142fd0(unsigned int value, const void *buffer,
                        unsigned int size, CtrHandle handle)
{
    struct IpcRequest_00142fd0 *command =
        (struct IpcRequest_00142fd0 *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(30u, 2u, 4u);
    /* Address-taking marshalling of the by-value words (docs/COMPILER.md,
     * middleware profile): qualification recorded in the row notes. */
    memcpy(&command->parameter, &value, sizeof(value));
    memcpy(&command->size, &size, sizeof(size));
    command->handle_descriptor = 0;
    memcpy(&command->handle, &handle, sizeof(handle));
    command->input.descriptor = ctrIpcStaticBufferDescriptor(size, 0);
    command->input.buffer = buffer;
    result = svcSendSyncRequest(data_00243040);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0)
        result = (CtrResult)command->parameter;
    return result;
}
