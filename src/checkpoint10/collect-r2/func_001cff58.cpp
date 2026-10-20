/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; algorithm after sources/libraries/lyt/lyt_Animation.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * GNU GPL version 3; see LICENSE.txt.
 *
 * [INFERENCE] Native spelling and role follow the already-admitted same-address
 * emitted self-definition, not the edited callers. The native list, transform
 * and returned link are shared with lookup and unbind consumers in game/layout.h.
 * The target owner prefixes are not aliases of native Pane or Material.
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
