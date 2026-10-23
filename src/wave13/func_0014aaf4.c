#include "game/material.h"
#include "game/screen.h"


void func_0014aaf4(struct BoundDisplayObject *object, int mode, float value)
{
    struct Material *material = func_0017fd68(object->bindings[0]->pane);
    struct MaterialPair pair;
    /* ORIGINAL BUG: the unused upper bits of the mode word are uninitialized. */
    pair.mode = mode;
    pair.value = value;
    *func_0014d254(material) = pair;
}
