#include "ctr/svc.h"
#include "game/runtime.h"
#include "game/service_state.h"

extern CtrResult func_0012ef30(void);
extern unsigned func_00121ddc(unsigned value);
extern unsigned func_00121da0(unsigned value);

static __forceinline unsigned result_failure(CtrResult result)
{
    return (unsigned)result >> 31;
}

/* INTEGRATOR: differs from src/wave4/func_0010c338.c: the tail-call result is returned to callers. */
unsigned func_0010c33c(void)
{
    unsigned state = (unsigned)func_001927a8();
    if (state != 0) {
        state = func_00192794();
        if (state == 0) {
            CtrResult result = func_0012ef30();
            if (result_failure(result))
                func_0019f568();
            func_00121ddc(15);
            return func_00121da0(6);
        }
    }
    return state;
}
