#include "game/material.h"
#include "game/shader.h"
extern "C" void func_0017fda4(DisplayPane *pane, Material *material)
{
    Material *previous = pane->material;
    if (previous == material)
        return;
    if (previous && !(previous->flags & 1)) {
        previous->vtable->destroy(previous);
        func_00191aec(previous);
    }
    pane->material = material;
    if (material)
        material->flags &= 0xfb;
}
