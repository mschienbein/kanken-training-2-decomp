#include "game/animation_binding_storage.h"

/* [INFERENCE] Factoring of the type reset, not recovered helper identity. */
static void restore_binding_type(AnimationBindingStorage *binding,
                                 const void *const *vtable)
{
    binding->vtable = vtable;
}

AnimationBindingStorage::~AnimationBindingStorage()
{
    restore_binding_type(this, data_00223e1c);
}
