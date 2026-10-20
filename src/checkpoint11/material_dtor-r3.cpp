#include "game/material_lifecycle.h"

static inline void unbind_material(MaterialObject *object)
{
    object->unbind_animation(0);
}

MaterialObject::~MaterialObject()
{
    unbind_material(this);
    if (storage) {
        func_00191aec(storage);
        storage = 0;
    }
}
