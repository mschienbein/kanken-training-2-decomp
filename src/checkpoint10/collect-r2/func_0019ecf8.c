#include "game/applet.h"
#include "game/service_state.h"
#include "game/waitable_handle.h"


static __inline unsigned failed(CtrResult result)
{
    return (unsigned)result >> 31;
}

unsigned func_0019ecf8(void)
{
    if (data_00243030 != 0) {
        CtrResult result = svcWaitSynchronization1(data_00243030, -1LL);
        if (failed(result))
            func_0019dfbc(result);
    }
    return func_00115b0c();
}
