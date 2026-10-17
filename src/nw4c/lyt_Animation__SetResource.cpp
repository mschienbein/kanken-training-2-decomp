/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_Animation.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: lyt_Animation.cpp
//
// Project: NintendoWare4Ctr

#include <nw/lyt/lyt_Common.h>
#include <nw/lyt/lyt_Pane.h>
#include <nw/lyt/lyt_Group.h>
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_ResourceAccessor.h>
#include <nw/lyt/lyt_Util.h>

const f32 R_SAME_TOLERANCE = 1.0e-5F;
const f32 R_FRAME_TOLERANCE = 0.001F;

namespace nw{
namespace lyt{
namespace{

/* Some Impl math shit */







/* Animate Inlines */

















}

/* AnimTransform */













/* AnimTransformBasic */





void AnimTransformBasic::SetResource(const res::AnimationBlock* pRes,ResourceAccessor* pResAccessor)
{
    NW_NULL_ASSERT(pRes);
    this->SetResource(pRes, pResAccessor, pRes->animContNum);
}











/* AnimResource */







}
}

