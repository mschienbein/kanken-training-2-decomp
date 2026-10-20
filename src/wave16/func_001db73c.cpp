#include "game/scene_lifecycle.h"

/* [INFERENCE] Scene lifecycle type and operation names. func_00141cd8 selects
 * this callback from data_0023eec0 into data_0023eeb0. func_0013db7c + 0x180
 * invokes it without arguments and discards its result. The virtual method
 * uses only the writable scene object. */

extern "C" void func_001db73c(void)
{
    ScreenResourceManager *scene = &bss_0028da50;
    scene->vtable->slot02(scene);
}
