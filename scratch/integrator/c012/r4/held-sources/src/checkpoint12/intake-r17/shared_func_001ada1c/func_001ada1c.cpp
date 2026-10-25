#include "game/resource_groups.h"

W22ResourceGroup *func_001ada1c(void)
{
    /* [INFERENCE] callee identity: static destructor registration and guard release were inlined by armlink. */
    static W22ResourceGroup group;
    return &group;
}
