#include "game/scheduler.h"
#include "game/startup.h"
/* C-only procedure callback: every caller discards its value (C11 6.9.1p12).
 * Do not consume the result or compile this definition as C++. */
int func_0010433c(void)
{
    unsigned char mode;

    if (func_001081ec() == 0)
        func_0019f324(1, 0);

    func_0019b54c(func_0019b564());
    mode = func_001081ec();
    if (mode == 1)
        func_00107a88(func_00107140);
    else if (mode == 11)
        func_00107a88(func_001070e8);
}
