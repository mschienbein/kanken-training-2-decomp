#include "game/stream_manager.h"

struct StreamManager *func_001916e8(void)
{
    if (!(data_0024371c.first & 1) && func_00191c80(&data_0024371c.first)) {
        struct StreamManager *manager = func_00143d1c(&bss_004b8640);
        /* [INFERENCE] Typed source-only no-op registration; original call
         * identity is erased. It neither stores nor invokes this callback. */
        register_stream_manager_cleanup(manager, func_001812f8, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_0024371c.first);
    }
    return &bss_004b8640;
}
