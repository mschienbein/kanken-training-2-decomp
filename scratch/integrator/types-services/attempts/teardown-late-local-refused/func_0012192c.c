#include "game/runtime.h"
#include "game/service_runtime.h"

void func_0012192c(void)
{
    int index;
    if (data_00242afc.session) {
        if (data_00242afc.busy)
            func_0019f568();
        for (index = 0; index < 8; ++index)
            if (bss_0049eec4[index])
                func_0019f568();
        CtrHandle *handle = &data_00242afc.handle;
        func_0012fbd4(&bss_0049ef24);
        svcCloseHandle(*handle);
        {
            const struct InvalidHandleRecord *invalid = &data_0021abd0;
            CtrHandle value = invalid->handle;
            data_00242afc.session = 0;
            *handle = value;
        }
        func_0012e620(&data_00242afc.lock);
    }
}
