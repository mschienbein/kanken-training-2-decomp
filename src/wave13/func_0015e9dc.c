#include "game/material.h"
#include "game/screen.h"


void func_0015e9dc(struct BoundDisplayObject *object, int index,
                   unsigned char alpha)
{
    struct ColoredDisplayBinding *binding =
        (struct ColoredDisplayBinding *)object->bindings[index];
    struct DisplayPane *pane;
    binding->alpha = alpha;
    pane = binding->holder->pane;
    func_0017aa90(pane, binding->red, binding->green, binding->blue, alpha);
}
