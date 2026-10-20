#include "game/functions.h"
#include "game/scheduler.h"


extern int func_0011f4b4(void);

/* C procedure counterpart: every caller discards its unspecified result. */
extern int func_00135f80(void);


/* C-only procedure callback: every caller discards its value (C11 6.9.1p12).
 * Do not consume the result or compile this definition as C++. */
int func_0012d328(void)
{
    int index = func_0019b564();
    if (func_0019b54c(index)) {
        if (!func_0011f4b4())
            func_00190688(index, func_00135ae8);
    } else {
        func_00186944(index, 1);
        func_00135f80();
    }
}
