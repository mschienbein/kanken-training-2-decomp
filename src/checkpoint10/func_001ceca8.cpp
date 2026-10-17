#include "game/runtime.h"

struct PoolOwner;
/* INTEGRATOR: move to include/game/pool_owned_list.h */
extern "C" {
PoolOwner *func_001cecb8(PoolOwner *owner);
}

extern "C" void func_001ceca8(PoolOwner *owner)
{
    func_001852f4(func_001cecb8(owner));
}
