/* INTEGRATOR: differs from include/game/render_state.h: the word at +4 is a control-flag field, not opaque padding. */
struct GpuFlushState {
    unsigned dirty;
    unsigned control_flags;
};
struct SubmissionState {
    unsigned command_mode;
    unsigned enabled;
    unsigned texture_state;
    unsigned feature_mask;
};
extern struct GpuFlushState *data_00242fd0;
extern struct SubmissionState *data_002429f0;
extern void func_00133b08(void);
extern void func_001876ac(struct GpuFlushState *state);
extern void func_001871e4(struct GpuFlushState *state);
extern void func_00186fc8(struct GpuFlushState *state);
extern void func_001395c0(struct GpuFlushState *state);

void func_0013cfdc(void)
{
    struct GpuFlushState *state = data_00242fd0;
    if (state->control_flags & 0x2000)
        func_00133b08();
    if (state->dirty & data_002429f0->command_mode)
        func_001876ac(state);
    if (state->dirty & data_002429f0->enabled)
        func_001871e4(state);
    if (state->dirty & data_002429f0->texture_state)
        func_00186fc8(state);
    if (state->dirty & data_002429f0->feature_mask)
        func_001395c0(state);
    state->dirty = 0;
}
