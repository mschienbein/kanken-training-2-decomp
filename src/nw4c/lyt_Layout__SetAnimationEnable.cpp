/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_Layout.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: lyt_Layout.cpp
//
// Project: NintendoWare4Ctr

#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_Bounding.h>
#include <nw/lyt/lyt_Common.h>
#include <nw/lyt/lyt_DrawInfo.h>
#include <nw/lyt/lyt_GraphicsResource.h>
#include <nw/lyt/lyt_Group.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Picture.h>
#include <nw/lyt/lyt_ResourceAccessor.h>
#include <nw/lyt/lyt_Resources.h>
#include <nw/lyt/lyt_TextBox.h>
#include <nw/lyt/lyt_Util.h>
#include <nw/lyt/lyt_Window.h>

#define ARRAY_LENGTH(a)   (sizeof(a) / sizeof((a)[0]))

namespace nw{
namespace lyt{
namespace{





}


void Layout::SetAnimationEnable(AnimTransform* pAnimTrans,bool bEnable)
{
    if (this->GetRootPane())
    {
        this->GetRootPane()->SetAnimationEnable(pAnimTrans, bEnable, true);
    }
}













}
}

