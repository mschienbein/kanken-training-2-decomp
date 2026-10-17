#include "game/runtime.h"
#include "game/service_state.h"

extern int func_00137f0c(unsigned flags);

void func_001904ec(void)
{
    int result = func_00137f0c(0x406);
    switch ((unsigned)result >> 31) {
    case 0:
        break;
    default:
        if ((unsigned)result != 0xc8a0cffc)
            func_0019f568();
        break;
    }
}
