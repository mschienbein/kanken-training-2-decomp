#include "game/service_runtime.h"
#include "game/input_runtime.h"
#include "game/display_animation.h"

CtrResult func_001821f4(struct ServiceOutput *state, int value)
{
    state->shared_status = value;
    return func_001462d4(&bss_004b97ac, value);
}
