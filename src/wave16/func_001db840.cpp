#include "game/scene_dispatch.h"

/* [INFERENCE] Shared scene-prefix type and virtual method names. */
/* Stored in the no-argument callback array at data_0023eec0.
 * Scene callback dispatchers discard their results.
 * [INFERENCE] The virtual result contract is void, as for the callback. */

extern "C" void func_001db840(void)
{
    ScreenResourceManager *scene = &bss_0028e848;
    scene->vtable->slot02(scene);
}
