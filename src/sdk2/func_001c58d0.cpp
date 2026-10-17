#include "game/kernel_owners.h"
DualHandleState *func_001c58d0(DualHandleState *object)
{
    object->state = -1;
    object->second.~ScopedKernelHandle();
    object->first.~ScopedKernelHandle();
    return object;
}
