#include "game/service_runtime.h"

#include "game/frame_timing.h"

void func_0014cad4(struct FrameTime *elapsed)
{
    struct ServiceWorker *worker = func_00182f88();
    func_0014caf0(elapsed, &worker->elapsed);
}
