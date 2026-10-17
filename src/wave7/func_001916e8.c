#include "game/stream_manager.h"

struct StreamManager *func_001916e8(void)
{
    if (!(data_0024371c.first & 1) && func_00191c80(&data_0024371c.first)) {
        struct StreamManager *manager = func_00143d1c(&bss_004b8640);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00216140(manager, (ObjectDestructor)func_001812f8, func_00100000);
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00215ec4(&data_0024371c.first);
    }
    return &bss_004b8640;
}
