#include "game/service_runtime.h"
void func_0014b224(struct ServiceWorker *state, unsigned char value)
{
    if (!state->skip_service)
        state->timed = value;
}
