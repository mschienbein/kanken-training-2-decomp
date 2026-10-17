#include "game/scheduler.h"

extern int func_0019f2e8(void);
extern void func_0013db40(void);
extern void func_00190688(int slot, void (*callback)(void));
extern void func_00186944(int task, int wait);

void func_00135ae8(void)
{
    int slot = func_0019b564();

    if (func_0019b54c(slot)) {
        if (!func_0019f2e8())
            func_00190688(slot, func_0013db40);
    } else {
        func_00186944(slot, 1);
    }
}
