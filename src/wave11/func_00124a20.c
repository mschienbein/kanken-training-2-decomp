#include "game/audio_tasks.h"
void func_00124a20(void)
{
    if (data_00242fb8.initialized) {
        /* [INFERENCE] callee identity: inlined by armlink */
        func_00137150(func_00185de4());
        func_00185d74(func_00140700());
        func_00183f08(func_00185de4());
        func_00131304(func_00185d1c());
        func_001312a4(func_00185d1c());
        func_00131398(func_00185cc0());
        func_00131464(func_00185c58());
        func_001313f4(&bss_004b32c0);
        func_00130f90(&data_00242fb8.work_area);
        func_00131330(func_00185d1c());
        func_00131284(func_001916e8());
        func_00131284(func_00185bfc());
        data_00242fb8.flag02 = 0;
        data_00242fb8.initialized = 0;
    }
}
