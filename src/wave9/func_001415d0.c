#include "game/startup.h"
#include "game/scheduler.h"

extern void func_00190688(int task, void (*callback)(void));

void func_001415d0(void)
{
    func_00190688(func_0019b564(), func_0014562c);
}
