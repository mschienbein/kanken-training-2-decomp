#include "game/material_lifecycle.h"

static __inline void unbind_material(struct MaterialObject *object)
{
    object->vtable->unbind_animation(object, 0);
}

void func_001cfe24(struct MaterialObject *object)
{
    object->vtable = &data_00224480;
    unbind_material(object);
    if (object->storage) {
        func_00191aec(object->storage);
        object->storage = 0;
    }
    func_001852f4(object);
}
