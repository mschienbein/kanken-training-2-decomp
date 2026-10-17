#include "game/service_availability.h"
#include "game/service_lifetime.h"
#include "game/service_state.h"
#include "ctr/svc.h"
#include "game/stream_manager.h"
#include "game/result.h"





static bool serviceHasReferences()
{
    ScopedServiceLock lock(&bss_0049ee70);
    return data_00242a1c.references != 0;
}

Result func_0010b804(void)
{
    ScopedServiceLock lock(&bss_0049ee70);
    if (serviceHasReferences()) {
        func_0012d478();
        if (--data_00242a1c.references == 0) {
            Result result = svcCloseHandle(data_00242ff8);
            unsigned failure = result.failed();
            if (failure)
                return result;
        }
        return 0;
    }
    return 0x209ff8;
}
