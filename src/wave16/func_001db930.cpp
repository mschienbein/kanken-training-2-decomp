#include "game/global_dispatch.h"

/* [INFERENCE] Table callback has no evidenced incoming arguments; the
 * selected virtual method receives the global object as this. */

void func_001db930(void)
{
    ScreenResourceManager *scene = &bss_0028fd24;
    scene->vtable->slot02(scene);
}
