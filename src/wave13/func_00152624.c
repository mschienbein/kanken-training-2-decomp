#include "game/input_device.h"
/* The caller builds two signed coordinates with halfword stores. */


void func_00152624(struct DirtyValueObject *object, const struct PackedPosition *value)
{
    object->value = *value;
    object->dirty |= 8;
}
