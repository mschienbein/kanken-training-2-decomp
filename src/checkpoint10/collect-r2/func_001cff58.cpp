/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; algorithm after sources/libraries/lyt/lyt_Animation.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * GNU GPL version 3; see LICENSE.txt.
 *
 * [INFERENCE] Identity: nw::lyt::internal::FindAnimationLink(AnimationList*,
 * AnimTransform*), declared in include/nw/lyt/lyt_Animation.h. The bytes confirm
 * the LinkList layout (count at +0, sentinel at +4, link node at +0, transform
 * at +8). Needs the proposed binding in names.json (mangled name -> 0x001cff58);
 * INTEGRATOR: differs from include/game/layout.h: migrate the C-linkage,
 * const-qualified LayoutAnimationList/LayoutAnimationTransform declaration
 * and its callers to this existing namespaced NW4C contract before adoption.
 */
#include <preinclude.h>
#include <nw/lyt/lyt_Animation.h>

namespace nw {
namespace lyt {
namespace internal {

AnimationLink* FindAnimationLink(AnimationList* pAnimList, AnimTransform* pAnimTrans)
{
    for (AnimationList::Iterator it = pAnimList->GetBeginIter(); it != pAnimList->GetEndIter(); ++it)
    {
        if (pAnimTrans == it->GetAnimTransform())
        {
            return &(*it);
        }
    }
    return 0;
}

} // namespace internal
} // namespace lyt
} // namespace nw
