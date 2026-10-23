#include "game/material.h"
#include "game/screen.h"

/* A texture binding uses a mode word where other binding kinds have a link. */

void func_00180404(struct BoundDisplayObject *object, int index)
{
    struct TextureDisplayBinding *binding =
        (struct TextureDisplayBinding *)object->bindings[index];
    binding->mode = 3;
}
