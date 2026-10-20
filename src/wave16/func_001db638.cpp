#include "game/scene_lifecycle.h"

/* [INFERENCE] Scene lifecycle type and operation names. The callback table at
 * data_0023ef14 is invoked without arguments at func_00135450 + 0x44;
 * its result is discarded. The resolved virtual method uses only the scene. */

extern "C" void func_001db638(void)
{
    ScreenResourceManager *scene = &bss_0028f200;
    scene->vtable->slot05(scene);
}
