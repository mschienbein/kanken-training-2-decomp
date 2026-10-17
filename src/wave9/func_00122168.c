#include "game/input_runtime.h"

int func_00122168(void)
{
    if (!data_00242d90.initialized)
        return 0;
    data_00242d90.initialized = 0;
    func_0012e854(func_0013e84c, func_0013e9d8, func_0013e810);
    func_001861e0();
    func_00137118();
    func_0012f1c0(&bss_004b5234);
    func_0012f1b4(&bss_004b6888);
    if (!data_00242d90.stopped)
        func_0013e894(&bss_004b5284, 0);
    return 0;
}
