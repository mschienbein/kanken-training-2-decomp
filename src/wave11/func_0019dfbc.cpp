#include "game/sdk_init.h"
#include "game/runtime.h"
extern "C" void func_0019dfbc(CtrResult result)
{
    unsigned level = (unsigned)result >> 27;
    const struct KernelConfiguration *configuration =
        (const struct KernelConfiguration *)0x1ff80000;
    /* Decode the signed five-bit level field without relying on signed shifts. */
    if (result & 0x80000000)
        level -= 32;
    if (!(configuration->environment & 1) && !data_00242d64) {
        if (level == (unsigned)-1)
            func_001219bc(result, 0, __return_address());
        func_0019f568();
        return;
    }
    if (level != (unsigned)-7 && level != 1)
        return func_001219bc(result, 0, __return_address());
}
