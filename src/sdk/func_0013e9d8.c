#include "ctr/cp15.h"
#include "game/service_runtime.h"

void func_0013e9d8(void)
{
    int status;
    if ((data_00242d90.initialized & data_00242d90.published) != 0) {
        func_00142838(&bss_004b5284, 1);
        status = ((const volatile struct SharedConfiguration *)0x1ff81000)->shared_status;
        data_00242d90.shared_status = (signed char)status;
        func_001821f4(&bss_004b5234, status);
        data_00242d90.published = 0;
        data_00242d90.pending = 0;
        ctrDataSynchronizationBarrier();
        func_0019ea6c(&data_00242d90.event);
    }
}
