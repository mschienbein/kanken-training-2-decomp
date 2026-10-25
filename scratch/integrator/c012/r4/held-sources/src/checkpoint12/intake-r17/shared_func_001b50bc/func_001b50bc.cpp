#include "game/resource_groups.h"

W22LargeResourceGroup *func_001b50bc(void)
{
    /* [INFERENCE] callee identity: static destructor registration and guard release were inlined by armlink. */
    static W22LargeResourceGroup group;
    return &group;
}
