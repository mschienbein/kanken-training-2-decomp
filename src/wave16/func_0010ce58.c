#include "game/functions.h"
#include "game/service_callbacks.h"

void func_0010ce58(int (*callback)(void *context), void *context)
{
    data_00242a6c.callbacks.poll = callback;
    data_00242a6c.contexts[5] = context;
}
