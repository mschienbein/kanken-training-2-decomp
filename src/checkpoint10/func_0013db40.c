#include "game/functions.h"
#include "game/startup.h"
#include "game/scheduler.h"


/* C-only procedure callback: the dispatcher MUST discard its unspecified
 * result (C11 6.9.1p12). Do not compile this definition as C++. */
int func_0013db40(void)
{
    /* [INFERENCE] callee identity: inlined by armlink */
    func_0012d1c8();
    func_0019f338(0x12, func_001415d0, 0x1100);
    func_0019eb60(0, 0);
    func_00190688(func_0019b564(), func_001415e8);
}
