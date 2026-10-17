#include "game/screen.h"

struct DisplayBindingTarget {
    unsigned char reserved00[0x10];
    float value;
};

/* INTEGRATOR: game/screen.h DisplayBinding padding contains a float at 0x08
 * and a target pointer at 0x20; expose these fields in the shared binding. */
struct DisplayBindingValue {
    unsigned char reserved00[8];
    float value;
    unsigned char reserved0c[0x14];
    struct DisplayBindingTarget *target;
};

void func_0015eac8(struct DisplayObject *object, int index, float value)
{
    struct BoundDisplayObject *bound = (struct BoundDisplayObject *)object;
    struct DisplayBindingValue *binding =
        (struct DisplayBindingValue *)bound->bindings[index];
    binding->value = value;
    binding->target->value = value;
}
