#include "game/transfer_stream.h"

static int result_module(Result result)
{
    return (result.value & 0x3fc00u) >> 10;
}

static int result_description(Result result)
{
    return result.value & 0x3ffu;
}

/* Local operation decomposition, not an inferred original-address callee. */
static unsigned transfer_at(int displacement, int position, unsigned size,
                            TransferStream *stream, unsigned char *buffer)
{
    unsigned transferred = 0;
    unsigned outcome = 0;
    Result result = stream->seek((long long)(position + displacement));
    if (result.value & 0x80000000u)
        goto failed;
    result = stream->read(&transferred, buffer + displacement, size);
    if (!(result.value & 0x80000000u))
        goto done;
failed:
    outcome = 1;
    if (result_module(result) == 17) {
        if (390 <= result_description(result)) {
            if (result_description(result) < 400)
                outcome = 2;
        }
    }
done:
    return outcome;
}

unsigned func_0019000c(TransferStream *stream, unsigned char *buffer,
                       int position, int displacement, unsigned size)
{
    return transfer_at(displacement, position, size, stream, buffer);
}
