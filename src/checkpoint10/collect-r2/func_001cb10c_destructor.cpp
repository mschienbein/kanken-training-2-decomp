/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr, commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd: gfx_AnimGroup.cpp,
 * gfx_AnimGroup.h and ut_MoveArray.h. NintendoWare4Ctr contributors.
 * Modified 2026-10-17: ABI storage view with complete zero initialization,
 * canonical resource/allocator/element types, paired construction/destruction.
 */
#include <preinclude.h>
#include <nw/anim/res/anim_ResAnim.h>
#include <nw/gfx/gfx_AnimGroup.h>
#include <new>

/* [INFERENCE] Storage view of the canonical AnimGroup: the native template
 * default constructor preserves capacity bits temporarily before clearing them.
 * This view expresses the same final wrapper-array state directly.
 */
/* INTEGRATOR: move the canonical AnimGroup ABI entry declarations to a shared
 * header and reconcile the older sound-track/ResourceArrays interpretations.
 */
extern "C" {
extern const unsigned data_0022409c;
}
struct R41270AnimGroupStorage {
    const void* vtable;
    nw::os::IAllocator* allocator;
    nw::anim::ResAnimGroup resource;
    nw::ut::MoveArray<nw::anim::AnimBlendOp*> blend_operations;
    nw::gfx::SceneNode* owner;
    nw::ut::MoveArray<int> target_indices;
    nw::ut::MoveArray<void*> target_objects;
    nw::ut::MoveArray<void*> target_pointers;
    nw::ut::MoveArray<const void*> original_values;
    nw::gfx::AnimGroup::PreEvaluateCallback callback;
    bool full_baked;
    R41270AnimGroupStorage(nw::anim::ResAnimGroup info, nw::gfx::SceneNode* node,
                          nw::os::IAllocator* heap)
        : vtable(&data_0022409c), allocator(heap), resource(info), owner(node),
          callback(0), full_baked(false) {}
    ~R41270AnimGroupStorage();
};
R41270AnimGroupStorage::~R41270AnimGroupStorage()
{
    vtable = &data_0022409c;
}
