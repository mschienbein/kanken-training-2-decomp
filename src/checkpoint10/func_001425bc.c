#include "game/service_runtime.h"
/* Caller contract: func_0013dec8 consumes bit 31 of this returned result;
 * the previous void declaration dropped an observed return value. */
#include "ctr/svc.h"


static __inline void configure_defaults(struct ResourceServiceState *state,
                                        const void *source, unsigned size)
{
    state->source = source;
    state->size = size;
    state->first = 0xff;
    state->second = 0xff;
}
CtrResult func_001425bc(void)
{
    CtrResult result;
    configure_defaults(&data_00242afc, data_00229aa0, 0xc26c);
    result = func_00185050(data_00242afc.source, data_00242afc.size,
                         data_00242afc.first, data_00242afc.second);
    if ((int)((unsigned)result & 0x80000000u) >= 0)
        data_00242afc.acquired = 1;
    return result;
}
