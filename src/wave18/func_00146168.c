#include "game/input_runtime.h"
#include "game/service_runtime.h"

struct InputSessionContext;
extern struct InputSessionContext bss_004b97ac;


void func_00146168(struct ServiceOutput *state, float value)
{
    if (state->active != 0) {
        state->volume = value;
        func_001487cc(&bss_004b97ac, value);
    }
}
