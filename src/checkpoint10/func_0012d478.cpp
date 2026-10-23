#include "game/service_lifetime.h"
#include "game/service_availability.h"
#include "game/result.h"
CtrResult func_0012d478(void)
{
    ScopedServiceLock guard(&bss_0049ee70);
    CtrResult result = 0;
    if (data_00242a1c.initialized) {
        result = func_0013e7d0();
        data_00242a1c.initialized = 0;
        Result close_result(func_00136d4c());
        switch (close_result.failed()) {
        case 0: break;
        default: result = close_result.value; break;
        }
    }
    return result;
}
