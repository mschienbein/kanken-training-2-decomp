#include "game/functions.h"
#include "game/scheduler.h"


extern int func_0011f4b4(void);

extern void func_00135f80(void);


void func_0012d328(void)
{
    int index = func_0019b564();
    if (func_0019b54c(index)) {
        if (!func_0011f4b4())
            func_00190688(index, func_00135ae8);
        return;
    }
    func_00186944(index, 1);
    func_00135f80();
}
