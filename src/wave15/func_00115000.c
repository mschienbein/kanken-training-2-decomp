#include "game/functions.h"
#include "game/service_callbacks.h"



void func_00115000(int wait)
{
    if (wait)
        func_00122ad0();
    if (data_00242a6c.callbacks.before)
        data_00242a6c.callbacks.before(data_00242a6c.contexts[13]);
    if (data_00242a6c.callbacks.after)
        data_00242a6c.callbacks.after(data_00242a6c.contexts[14]);
}
