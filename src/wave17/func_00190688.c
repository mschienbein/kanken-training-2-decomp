#include "game/scheduler.h"

/* [INFERENCE] The frozen sdk profile reproduces this binding copy exactly;
 * subsystem/compiler provenance is not established merely by a byte match. */
void func_00190688(unsigned index, int (*callback)(void))
{
    struct ScheduledTaskBinding binding;
    binding.callback = callback;
    binding.wait = 0;
    bss_00495c8c.tasks[index].binding = binding;
}
