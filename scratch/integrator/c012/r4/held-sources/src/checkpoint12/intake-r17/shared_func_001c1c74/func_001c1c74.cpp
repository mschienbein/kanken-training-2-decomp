#include "game/runtime_singletons.h"

SingletonHolderW22D *func_001c1c74(void)
{
    /* [INFERENCE] callee identities: static destruction registration and
     * guard release were inlined by armlink; erased destinations are unrecoverable. */
    static SingletonHolderW22D holder_001c1c74;
    return &holder_001c1c74;
}
