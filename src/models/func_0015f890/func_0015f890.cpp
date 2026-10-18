#include "game/text_resources.h"
/* [INFERENCE] The empty base-destruction boundary may have been armlink-inlined
 * and removed; its historical identity and original outlining are unknown. */

void func_0015f890(W22TextResources *resources)
{
    resources->vtable = data_00223508;
    func_0015f5e4(resources);
    func_0019dc1c(completeTextResourceDestruction(resources));
}
