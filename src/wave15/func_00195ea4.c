#include "game/object_manager.h"
#include "game/service_runtime.h"



void func_00195ea4(struct HandleManager *state)
{
    state->queued = 0;
    state->completed = 0;
    state->pending = 0;
    state->identifier = 0;
    state->active = 0;
    func_00195ec4(&state->mutex);
}
