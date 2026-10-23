#include "ctr/ipc.h"
#include "game/archive_ipc.h"

union IpcWord {
    unsigned int word;
    unsigned char byte;
};

struct ApplicationBufferCommand {
    unsigned int header;
    union IpcWord mode;
    unsigned long long id;
    unsigned int count;
    unsigned int input_descriptor;
    const unsigned short *input;
    unsigned int output_descriptor;
    void *output;
};

unsigned func_00136340(void *output, unsigned char mode,
                      unsigned long long id, const unsigned short *input,
                      unsigned int count)
{
    struct ApplicationBufferCommand *command =
        (struct ApplicationBufferCommand *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    unsigned int input_bytes;
    command->header = (0x1002u << 16) | (4u << 6) | 4u;
    ctrIpcMarshalByte(&command->mode.byte, &mode);
    command->id = id;
    command->count = count;
    input_bytes = count * sizeof(*input);
    command->input_descriptor = ctrIpcMappedBufferDescriptor(input_bytes, 2u);
    command->input = input;
    command->output_descriptor = ((count * 3u) << 7) | 12u;
    command->output = output;
    result = svcSendSyncRequest(data_0024377c);
    if (ctrIpcTransportFailure(result) >= 0)
        result = (CtrResult)command->mode.word;
    return result;
}
