#include "game/exp_heap.h"
#include "game/functions.h"
#include "game/startup.h"

/* C-only procedure callback: every caller discards its value (C11 6.9.1p12).
 * Do not consume the result or compile this definition as C++. */
int func_001070e8(void)
{
    func_00195b6c(0, 0, 0, 0, 0);
    func_00110ed8(1024);
    func_00110eac(1024);
    func_00110a58();
    func_0019f338(16, func_001109fc, 4096);
}
