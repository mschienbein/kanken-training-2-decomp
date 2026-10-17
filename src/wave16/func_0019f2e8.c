#include "game/task_runtime.h"
struct WidgetReadinessState {
    unsigned char reserved00[0x708];
    int enabled;
    int ready;
};

extern struct WidgetReadinessState bss_002d0540;

int func_0019f2e8(void)
{
    if (!bss_0029f540.active)
        return 0;
    if (!bss_002d0540.enabled)
        return 0;
    return bss_002d0540.ready != 0;
}
