#include "game/graphics_resources.h"
static __forceinline GraphicsResource *createResource(GraphicsDescription description)
{
    return func_001936a0(&description, &data_002426d0);
}

void func_00108f94(void)
{
    bss_0049da58.resources[0] = 0;
    bss_0049da58.resources[1] = 0;
    bss_0049da58.resources[2] = 0;
    bss_0049da58.resources[0] = createResource(GraphicsDescription());
    bss_0049da58.resources[1] = createResource(GraphicsDescription());
    bss_0049da58.resources[2] = createResource(GraphicsDescription());
}
