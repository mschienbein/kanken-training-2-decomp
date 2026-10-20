#include "ctr/svc.h"
#include "game/runtime.h"
#include "game/service_state.h"

#include "game/service_availability.h"

static __forceinline unsigned result_failure(Result result)
{
    return result.failed();
}

unsigned func_0010c33c(void)
{
    unsigned state = (unsigned)func_001927a8();
    if (state != 0) {
        state = func_00192794();
        if (state == 0) {
            Result result = func_0012ef30();
            if (result_failure(result))
                func_0019f568();
            func_00121ddc(15);
            return func_00121da0(6);
        }
    }
    return state;
}
