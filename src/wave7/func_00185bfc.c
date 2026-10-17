#include "game/stream_manager.h"

struct StreamManager *func_00185bfc(void)
{
    if (!(data_0024371c.second & 1) && func_00191c80(&data_00243720)) {
        struct StreamManager *manager = func_00143d1c(&bss_004b87d0);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00216140(manager, (ObjectDestructor)func_001812f8, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_00243720);
    }
    return &bss_004b87d0;
}
