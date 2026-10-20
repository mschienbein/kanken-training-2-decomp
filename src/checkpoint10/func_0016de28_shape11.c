#include "game/w11_control.h"
static __forceinline unsigned char disabled_value(int enabled)
{
    if (enabled) return 0;
    return 1;
}
void func_0016de28(struct AnimationState *state, int enabled)
{
    func_0016f208(state, 0, enabled);
    func_0016f208(state, 2, enabled);
    if (state->has_secondary) {
        func_0016f208(state, 1, enabled);
        func_0016f208(state, 3, enabled);
    }
    state->disabled = disabled_value(enabled);
}
