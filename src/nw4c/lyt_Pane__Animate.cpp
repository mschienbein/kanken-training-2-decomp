/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_Pane.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: lyt_Pane.cpp
//
// Project: NintendoWare4Ctr

#include <nw/lyt/lyt_Pane.h>
#include <nw/lyt/lyt_Common.h>
#include <nw/lyt/lyt_DrawInfo.h>
#include <nw/lyt/lyt_GraphicsResource.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_Material.h>

namespace nw { 
namespace lyt {
namespace internal {

} // namespace internal

void Pane::Animate(u32 option) 
{
    AnimateSelf(option);

    if (IsVisible() || !(option & ANIMATEOPT_NOANIMATEINVISIBLE)) 
    {
        for (PaneList::Iterator it = m_ChildList.GetBeginIter(); it != m_ChildList.GetEndIter(); ++it)
        {
            it->Animate(option);
        }
    }
}

inline void Pane::AnimateSelf(u32 option) 
{
    for (AnimationList::Iterator it = m_AnimList.GetBeginIter(); it != m_AnimList.GetEndIter(); ++it)
    {
        if (it->IsEnable())
        {
            AnimTransform* animTrans = it->GetAnimTransform();
            animTrans->Animate(it->GetIndex(), this);
        }
    }

    if (IsVisible() || !(option & ANIMATEOPT_NOANIMATEINVISIBLE)) 
    {
        u32 nbMaterial = GetMaterialNum();
        for (u32 idx = 0; idx < nbMaterial; ++idx)
        {
            Material* pMaterial = GetMaterial(idx);
            if (pMaterial)
            {
                pMaterial->Animate();
            }
        }
    }
}

inline Material* Pane::GetMaterial() const
{
    if (GetMaterialNum() > 0)
    {
        return GetMaterial(0);
    }
    else
    {
        return 0;
    }
}

inline Material* Pane::GetMaterial(u32 idx) const
{
    return NULL;
}

inline u8 Pane::GetMaterialNum() const
{
    return 0;
}

} // namespace lyt
} // namespace nw

