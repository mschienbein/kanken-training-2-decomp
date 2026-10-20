#include "ctr/ipc.h"

union ConnectionCommand {
    struct {
        unsigned header;
        unsigned size;
        unsigned descriptor;
        void *buffer;
    } request;
    struct {
        unsigned header;
        CtrResult result;
    } response;
};

/* [INFERENCE] Stage the command's header and static-buffer index together. */
struct ConnectionOptions {
    unsigned header;
    unsigned static_buffer_index;
};

CtrResult func_00138274(void *buffer, unsigned int size)
{
    union ConnectionCommand *command =
        (union ConnectionCommand *)ctrIpcCommandBuffer();
    struct ConnectionOptions options;
    CtrResult result;
    options.header = ctrIpcHeader(64u, 1u, 2u);
    options.static_buffer_index = 0;
    command->request.header = options.header;
    command->request.size = size;
    command->request.descriptor =
        ctrIpcStaticBufferDescriptor(size, options.static_buffer_index);
    command->request.buffer = buffer;
    result = svcSendSyncRequest(data_00243040);
    if (ctrIpcTransportFailure(result) >= 0)
        result = command->response.result;
    return result;
}
