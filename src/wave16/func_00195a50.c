#include "game/functions.h"
#include "game/startup.h"
#include "game/scheduler.h"






extern void func_00197220(void);

extern void func_00135058(void);



extern void func_00195b6c(int mode, void (*initialize)(void), void (*callback)(void),
                         void (*complete)(void), int argument);
/* func_001109fc/func_00110a28 pass this shutdown callback, not an integer. */


void func_00195a50(void)
{
    if (func_001869d0(17))
        func_00195ad0(17);
    if (func_001869d0(18))
        func_00195ad0(18);
    func_001360a4();
    func_00135a28();
    func_00135a58();
    func_00135a88();
    func_00197220();
    func_00135ab8();
    func_00135058();
    func_001351bc();
    func_0012d2bc();
    func_0012d2e8();
    func_00195b6c(0, 0, 0, 0, 0);
    func_001906a8(0);
    func_00195ad0(16);
}
