#include "game/functions.h"
#include "game/screen.h"
#include "game/scheduler.h"
#include "game/startup.h"


extern void func_0012d18c(void);



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
