#include "game/stream_manager.h"

struct StreamManager *func_00185bfc(void)
{
    if (!(data_0024371c.second & 1) && func_00191c80(&data_00243720)) {
        struct StreamManager *manager = func_00143d1c(&bss_004b87d0);
        /* [INFERENCE] Typed source-only no-op registration; original call
         * identity is erased. It neither stores nor invokes this callback. */
        register_stream_manager_cleanup(manager, func_001812f8, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_00243720);
    }
    return &bss_004b87d0;
}
