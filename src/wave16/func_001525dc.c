#include "game/service_runtime.h"
#include "game/aux_effect.h"
/* [INFERENCE] The nullable workspace is forwarded unchanged to the publisher;
 * its canonical layout is established by the initializer and clear paths. */

int func_001525dc(signed char index, struct ExtendedWorkspace *workspace)
{
    return func_001534e0(&bss_004b5234, index, workspace);
}
