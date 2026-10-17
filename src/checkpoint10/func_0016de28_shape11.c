/* INTEGRATOR: differs from include/game/w11_control.h: this routine
 * accesses two navigation-state flags within the formerly opaque prefix. */
struct AnimationState {
    unsigned char reserved00[0x34];
    unsigned char disabled;
    unsigned char has_secondary;
    unsigned char reserved36[0x0e];
    float start;
    float finish;
    unsigned char reserved4c[0x0c];
    float lower;
    float upper;
    float current;
};

extern void func_0016f208(struct AnimationState *state, int index, int enabled);
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
