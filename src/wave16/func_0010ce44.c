#include "game/functions.h"
#include "game/service_callbacks.h"

void func_0010ce44(void (*callback)(void *context), void *context)
{
    data_00242a6c.callbacks.stop = callback;
    data_00242a6c.contexts[8] = context;
}
