#include "game/waitable_handle.h"

struct WorkerThread *func_00143ac8(struct WorkerThread *worker)
{
    if (worker->running)
        func_00185d74(worker);
    func_0019df84(&worker->thread);
    if (worker->thread.handle != 0) {
        svcCloseHandle(worker->thread.handle);
        worker->thread.handle = 0;
    }
    return worker;
}
