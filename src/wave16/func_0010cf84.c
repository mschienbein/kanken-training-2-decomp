#include "game/service_callbacks.h"

void func_0010cf84(void (*callback)(void *context), void *context)
{
    data_00242a6c.callbacks.suspend = callback;
    data_00242a6c.contexts[6] = context;
}
