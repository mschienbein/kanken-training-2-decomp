#include "ctr/ipc.h"
#include "game/service_state.h"

/* Kernel IPC is serialized in words; never view it as an unrelated runtime object. */
static void marshal_handle_fields(unsigned *command, unsigned flags,
                                  const unsigned &handle_descriptor);
static CtrResult request_result(CtrResult result, unsigned *command);

CtrResult func_0013e260(const void *buffer, unsigned flags)
{
    unsigned *command = ctrIpcCommandBuffer();
    command[0] = ctrIpcHeader(4, 0, 6);
    const unsigned handle_descriptor = 0;
    marshal_handle_fields(command, flags, handle_descriptor);
    command[5] = ctrIpcStaticBufferDescriptor(512, 1);
    command[6] = reinterpret_cast<unsigned>(buffer);
    return request_result(svcSendSyncRequest(data_00242ff8), command);
}

static CtrResult request_result(CtrResult result, unsigned *command)
{
    int failure = ctrIpcTransportFailure(result);
    if (failure >= 0) result = static_cast<CtrResult>(command[1]);
    return result;
}

static void marshal_handle_fields(unsigned *command, unsigned flags, const unsigned &handle_descriptor)
{
    command[1] = 0x20;
    command[3] = handle_descriptor;
    command[4] = flags;
}
