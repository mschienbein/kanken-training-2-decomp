#include "ctr/cp15.h"
#include "game/stream_manager.h"

extern void func_0014e6c8(struct StreamEvent *event);
extern void func_00183900(struct StreamQueue *queue, struct StreamEvent *event);

void func_0014d298(struct StreamManager *manager)
{
    struct StreamEvent *event;
    while (func_001837c8(&manager->requests, &event)) {
        func_0014e6c8(event);
        ctrDataMemoryBarrier();
        func_00183900(&manager->completions, event);
    }
}
