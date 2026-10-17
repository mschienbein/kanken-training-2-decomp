#include "game/input_runtime.h"
#include "game/service_runtime.h"

struct InputSessionContext;
extern struct InputSessionContext bss_004b97ac;
extern void func_0014cbf4(struct InputSessionContext *state, float value);

void func_0014b154(struct ServiceOutput *state, float value)
{
    if (state->active != 0) {
        state->first_gain = value;
        func_0014cbf4(&bss_004b97ac, value);
    }
}
