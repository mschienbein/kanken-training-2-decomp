#include "game/service_runtime.h"
#include "game/service_state.h"
static LifecycleCallback *ExitCallbacks(void)
{
    return bss_0049eee4.exiting;
}

int func_0012e854(LifecycleCallback first, LifecycleCallback second,
                  LifecycleCallback third)
{
    int i;
    for (i = 0; i < 8; ++i) {
        if (bss_0049eec4[i] == first) {
            bss_0049eec4[i] = 0;
            bss_0049eee4.resumed[i] = 0;
            ExitCallbacks()[i] = 0;
            return 1;
        }
    }
    return 0;
}
