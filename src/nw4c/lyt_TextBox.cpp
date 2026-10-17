/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/lyt/lyt_TextBox.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: lyt_TextBox.cpp
//
// Project: NintendoWare4Ctr

#include <nw/lyt/lyt_DrawInfo.h>
#include <nw/lyt/lyt_GraphicsResource.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_TextBox.h>
#include <nw/lyt/lyt_Material.h>
#include <nw/lyt/lyt_Animation.h>
#include <nw/lyt/lyt_Common.h>
#include <nw/lyt/lyt_ResourceAccessor.h>
#include <nw/font/font_DispStringBuffer.h>

#include <nn/math/math_Arithmetic.h>

namespace nw{
namespace lyt{
namespace internal{
namespace{





}
}

using namespace math;

u8 TextBox::GetMaterialNum() const
{
    return m_pMaterial? 1 : 0;
}

Material* TextBox::GetMaterial(u32 idx) const
{
    return idx == 0 ? m_pMaterial : 0;
}





void TextBox::SetVtxColor(u32 idx,ut::Color8 value)
{
    this->SetTextColor(idx / 2, value);
}



void TextBox::SetVtxColorElement(u32 idx, u8 value)
{
    u8& elm =
        reinterpret_cast<u8*>(&this->m_TextColors[idx / (2 * sizeof(ut::Color8))])[idx % sizeof(ut::Color8)];
    elm = value;
}























void TextBox::GetTextGlobalMtx(nw::math::MTX34* pMtx) const
{
    MTX34Copy(pMtx, &GetGlobalMtx());

    VEC2 pos = GetVtxPos();
    const VEC2 txtPos = AdjustTextPos(GetSize(), false);

    pos.x += txtPos.x;
    pos.y -= txtPos.y;

    pMtx->m[0][3] += pMtx->m[0][0] * pos.x + pMtx->m[0][1] * pos.y;
    pMtx->m[1][3] += pMtx->m[1][0] * pos.x + pMtx->m[1][1] * pos.y;
    pMtx->m[2][3] += pMtx->m[2][0] * pos.x + pMtx->m[2][1] * pos.y;

    pMtx->m[0][1] = - pMtx->m[0][1];
    pMtx->m[1][1] = - pMtx->m[1][1];
    pMtx->m[2][1] = - pMtx->m[2][1];
}





}
}

