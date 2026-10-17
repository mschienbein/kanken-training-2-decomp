#include "ctr/cp15.h"
#include "game/service_runtime.h"

void func_0013e810(void)
{
    if ((data_00242d90.initialized & data_00242d90.published) != 0) {
        data_00242d90.stopped = 1;
        data_00242d90.published = 0;
        data_00242d90.pending = 0;
        ctrDataSynchronizationBarrier();
        func_0019ea6c(&data_00242d90.event);
    }
}
