#ifndef GAME_ANIMATION_BINDING_STORAGE_H
#define GAME_ANIMATION_BINDING_STORAGE_H
#include <preinclude.h>
#include <nw/gfx/gfx_AnimObject.h>
/* [INFERENCE] Partial destructor owner. Native MoveArray element types and
 * allocator reference follow the canonical AnimBinding/GfxObject prefix. */
struct AnimationBindingStorage {
    const void *const *vtable;
    nw::os::IAllocator &allocator;
    nw::ut::MoveArray<nw::gfx::AnimGroup *> groups;
    nw::ut::MoveArray<nw::gfx::AnimObject *> objects;
    const int objectsPerGroup;
    ~AnimationBindingStorage();
};
extern "C" { extern const void *const data_00223e1c[]; }
#endif
