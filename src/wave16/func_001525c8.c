#include "game/service_runtime.h"
#include "game/aux_effect.h"
/* [INFERENCE] The nullable workspace is forwarded unchanged to the publisher;
 * its canonical layout is established by the initializer and clear paths. */

int func_001525c8(signed char index, struct ChannelWorkspace *workspace)
{
    return func_0015343c(&bss_004b5234, index, workspace);
}
