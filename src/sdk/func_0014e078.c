#include "ctr/cp15.h"
#include "game/service_runtime.h"

void func_0014e078(void)
{
    struct ServiceWorker *worker = func_00182f88();
    worker->service_running = 1;
    do {
        func_00195d94(&worker->request);
        if (worker->service)
            worker->service(worker->service_context);
        func_00182210();
        if (func_0018f018()) {
            func_0017e7a0(&bss_004b5234, 0,
                bss_004b5284.buffers[bss_004b5284.current_buffer].first);
            func_0017e7a0(&bss_004b5234, 1,
                bss_004b5284.buffers[bss_004b5284.current_buffer].second);
        }
        func_00182204();
        ctrDataSynchronizationBarrier();
        func_0019ea6c(&worker->completion);
    } while (worker->service_running);
}
