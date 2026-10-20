#include "ctr/ipc.h"

struct R4023StaticInputBuffer {
    unsigned int descriptor;
    const void *buffer;

    R4023StaticInputBuffer(unsigned int size, const void *pointer,
                          unsigned int index = 0)
        : descriptor(ctrIpcStaticBufferDescriptor(size, index)), buffer(pointer) {}
};
struct R4023BufferCommand {
    unsigned int header;
    unsigned int selector;
    unsigned int size;
    unsigned int handle_descriptor;
    CtrHandle handle;
    R4023StaticInputBuffer input;
};


/* Qualifications: ctr/cp15.h TLS read and ctr/svc.h svcSendSyncRequest. */
CtrResult func_001235a8(unsigned int selector, const void *buffer,
                       unsigned int size, CtrHandle handle)
{
    R4023BufferCommand *command =
        reinterpret_cast<R4023BufferCommand *>(ctrIpcCommandBuffer());
    CtrResult result;

    command->header = ctrIpcHeader(31, 2, 4);
    ctrIpcMarshal(command->selector, selector);
    ctrIpcMarshal(command->size, size);
    command->handle_descriptor = 0;
    ctrIpcMarshal(command->handle, handle);
    command->input = R4023StaticInputBuffer(size, buffer);
    result = svcSendSyncRequest(data_00243040.handle);
    if (ctrIpcTransportFailure(result) >= 0)
        result = static_cast<CtrResult>(command->selector);
    return result;
}
