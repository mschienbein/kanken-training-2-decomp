/* INTEGRATOR: differs from include/game/service_state.h: PendingState also has a writable completion flag at +4. */
struct PendingState {
    unsigned char reserved00;
    unsigned char pending;
    signed char blocking;
    unsigned char reserved03;
    unsigned char completion;
};
extern struct PendingState data_00242a28;
extern void func_001368b0(void);
extern int func_0018f018(void);
extern void func_0018ef74(void);

void func_00185f70(int enabled)
{
    if (enabled) {
        if (data_00242a28.completion) {
            func_001368b0();
            data_00242a28.completion = 0;
        }
    } else {
        if (func_0018f018()) {
            func_0018ef74();
            data_00242a28.completion = 1;
        }
    }
}
