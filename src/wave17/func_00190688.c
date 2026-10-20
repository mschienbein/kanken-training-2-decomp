#include "game/scheduler.h"

static void assign_binding(struct ScheduledTaskBinding *destination,
                           const struct ScheduledTaskBinding *binding)
{
    *destination = *binding;
}

/* [INFERENCE] The frozen sdk profile reproduces this binding copy exactly;
 * subsystem/compiler provenance is not established merely by a byte match. */
/* C-only procedure: callers MUST discard its unspecified result (6.9.1p12). */
int func_00190688(int index, ScheduledTaskCallback callback)
{
    struct ScheduledTaskBinding binding;
    binding.callback = callback;
    binding.wait = 0;
    assign_binding(&bss_00495c8c.tasks[index].binding, &binding);
}
