#include "game/runtime_singletons.h"

extern "C" ServiceSingletonW22C *func_001ebda4(void)
{
    /* [INFERENCE] Singleton class identity is unrecovered. The empty destructor
     * registration and guard-release callees were inlined by armlink. */
    static ServiceSingletonW22C instance;
    return &instance;
}
