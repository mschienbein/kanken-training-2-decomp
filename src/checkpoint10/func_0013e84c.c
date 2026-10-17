#include "game/sound_task.h"
#include "game/service_runtime.h"



void func_0013e84c(void)
{
    if (data_00242d90.initialized & ~data_00242d90.published) {
        func_0019df28(&data_00242d90.event, 1);
        data_00242d90.pending = 1;
        func_0013e894(&bss_004b5284, 1);
        data_00242d90.published = 1;
    }
}
