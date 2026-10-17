struct TransitionState {
    unsigned char reserved00[0x18];
    int active;
};
extern struct TransitionState bss_00290c90;
extern int func_0019b564(void);
extern int func_0019b54c(int task);
extern int func_0019efd0(void);
extern void func_0012d18c(void);
extern void func_00136104(void);
extern void func_0012d1c8(void);
extern int func_00141c70(void);
extern void func_0019f338(int task, void (*callback)(void), unsigned priority);
extern void func_00195b6c(int mode, void (*initialize)(void), void (*callback)(void),
                         void (*complete)(void), int argument);
extern void func_00190688(int task, void (*callback)(void));
extern void func_00186944(int task, int wait);

void func_0011f234(void)
{
    int task = func_0019b564();
    if (func_0019b54c(task)) {
        if (!func_0019efd0()) {
            bss_00290c90.active = 0;
            func_0019f338(0x12, func_0012d18c, 0x1100);
            func_00195b6c(1, func_00136104, func_0012d1c8, 0, 0);
            /* The scheduler discards callback return values. */
            func_00190688(0x10, (void (*)(void))func_00141c70);
        }
    } else {
        func_00186944(task, 1);
    }
}
