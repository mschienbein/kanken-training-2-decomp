#include "game/screen.h"
#include "game/display_animation.h"

void func_001c1ddc(struct DisplayRegistration *const *panel, int index, float x, float y)
{
    struct DisplayRegistration *slot;
    struct DisplayObject *object;
    /* ORIGINAL BUG: the upper-bound check accepts negative indices. */
    if (index >= 1)
        return;
    func_001c1e6c(panel, index, 1);
    slot = panel[index];
    if (slot == 0)
        return;
    /* ORIGINAL BUG: a slot with no display object is dereferenced. */
    object = slot->object;
    object->position[0] = x;
    object->position[1] = y;
    switch (func_0017a9a0(object, 0)) {
    case 5:
        func_0015e684((struct BoundDisplayObject *)object, 0,
                      object->position[0], object->position[1], object->position[2]);
        break;
    case 6:
        func_00181700((struct BoundDisplayObject *)object,
                      object->position[0], object->position[1], object->position[2]);
        break;
    }
}
