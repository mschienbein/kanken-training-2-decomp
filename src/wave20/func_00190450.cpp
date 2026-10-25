#include "game/service_state.h"
#include "game/service_protocol.h"
#include "game/runtime.h"
#include "game/result.h"

static bool result_failed(const Result &result)
{
    return (result.value & 0x80000000u) != 0;
}

/* [INFERENCE] A reply-store helper is not a recovered historical identity. */
static void store_reply(unsigned char reply, unsigned char *destination)
{
    *destination = reply;
}

void func_00190450(unsigned char *output, struct RequestRecord *record)
{
    struct RequestRecord *request = record;
    void *memory;
    unsigned received;
    CtrResult result;
    memory = 0;
    received = 0;
    result = func_0013ef10(0x406);
    if (result_failed(Result(result))) {
        if ((unsigned)result != 0xc8a0cffc)
            func_0019f568();
    }
    func_0013ed94(0x406, request, 0xf80, data_00242a34);
    struct AppletTimeout timeout;
    timeout.nanoseconds = data_00242a40.nanoseconds;
    store_reply((unsigned char)func_00191e18(&received, request, 0xf80,
                                           &memory, 0, timeout), output);
}
