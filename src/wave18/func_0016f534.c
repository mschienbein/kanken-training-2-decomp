#include "game/screen.h"

void func_0016f534(struct BoundDisplayObject *object, int index, float duration)
{
    struct DisplayBinding *binding = object->bindings[index];
    if (duration == -1.0f) {
        binding->mode = 2;
    } else {
        binding->duration = duration;
        binding->mode = 1;
    }
}
