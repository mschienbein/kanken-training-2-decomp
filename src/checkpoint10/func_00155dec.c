#include "game/service_runtime.h"

/* Same shapes as nw::ut::Min/Max (include/nw/ut/ut_Inlines.h). */
static __inline int Max(int a, int b) { return a < b ? b : a; }
static __inline int Min(int a, int b) { return b < a ? b : a; }

void func_00155dec(struct AudioChannel *channel, int priority)
{
    priority = Min(priority, 0x7fff);
    priority = Max(priority, 0);
    channel->priority = priority;
    func_00156484(&bss_004b6888, channel, priority);
}
