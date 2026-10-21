#include "game/global_dispatch.h"

/* [INFERENCE] Table callback has no evidenced incoming arguments; the
 * selected virtual method receives the global object as this. */

void func_001db930(void)
{
    MenuScene *scene = &bss_0028fd24;
    scene->build();
}
