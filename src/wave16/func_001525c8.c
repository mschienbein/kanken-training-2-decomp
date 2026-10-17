#include "game/service_runtime.h"
#include "game/aux_effect.h"
/* [INFERENCE] Opaque manager/resource identities; the index and nullable
 * resource pointer are established by the forwarding call and its callee. */
struct ServiceOutput;
struct AuxDelayProcessor;
extern int func_0015343c(struct ServiceOutput *manager, signed char index,
                         struct AuxDelayProcessor *resource);

int func_001525c8(signed char index, struct AuxDelayProcessor *resource)
{
    return func_0015343c(&bss_004b5234, index, resource);
}
