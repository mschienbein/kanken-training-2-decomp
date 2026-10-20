#include "game/service_runtime.h"

struct FrameTime { long long ticks; };
extern void func_0014caf0(struct FrameTime *destination, const long long *source);

void func_0014cad4(struct FrameTime *elapsed)
{
    struct ServiceWorker *worker = func_00182f88();
    func_0014caf0(elapsed, &worker->elapsed);
}
