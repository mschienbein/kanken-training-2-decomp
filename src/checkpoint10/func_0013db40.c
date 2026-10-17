#include "game/functions.h"
#include "game/startup.h"
#include "game/scheduler.h"

extern void func_001415d0(void);
extern int func_001415e8(void);
extern void func_0019f338(int task, void (*callback)(void), unsigned priority);



void func_0013db40(void)
{
    /* [INFERENCE] callee identity: inlined by armlink */
    func_0012d1c8();
    func_0019f338(0x12, func_001415d0, 0x1100);
    func_0019eb60(0, 0);
    /* The scheduler discards callback return values. */
    func_00190688(func_0019b564(), (void (*)(void))func_001415e8);
}
