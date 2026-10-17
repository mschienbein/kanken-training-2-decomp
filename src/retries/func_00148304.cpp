#include "game/service_workers.h"
#include "game/waitable_handle.h"
#include "ctr/svc.h"




__forceinline ThreadHandle::~ThreadHandle()
{
    ThreadHandle *thread = this;
    func_0019df84(thread);
    CtrHandle *value = &thread->handle;
    if (*value != 0) {
        svcCloseHandle(*value);
        *value = 0;
    }
}

ServiceWorkers *func_00148304(ServiceWorkers *workers)
{
    workers->auxiliary_thread.~ThreadHandle();
    workers->main_thread.~ThreadHandle();
    return workers;
}
