#include "game/service_runtime.h"
#include "game/aux_effect.h"
/* [INFERENCE] Opaque manager/resource identities; the index and nullable
 * resource pointer are established by the forwarding call and its callee. */
struct ServiceOutput;
struct AuxReverbProcessor;
extern int func_001534e0(struct ServiceOutput *manager, signed char index,
                         struct AuxReverbProcessor *resource);

int func_001525dc(signed char index, struct AuxReverbProcessor *resource)
{
    return func_001534e0(&bss_004b5234, index, resource);
}
