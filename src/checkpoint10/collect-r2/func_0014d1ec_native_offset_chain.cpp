/* SPDX-License-Identifier: GPL-3.0-only
 * Adapted from ctrdecomp/NintendoWare4Ctr 15ddb5632db5a6f0d2f46e3827fc78df468684bd,
 * sources/libraries/lyt/lyt_Material.cpp; selected native helper chain only.
 */
#include <preinclude.h>
#include <nw/lyt/lyt_Common.h>
#include <nw/lyt/lyt_Material.h>
namespace nw { namespace lyt { namespace {
u32 CalcOffsetTexSRTAry(const internal::MatMemNums& bitNums)
{ return sizeof(TexMap) * bitNums.texMap; }
u32 CalcOffsetTexCoordGenAry(const internal::MatMemNums& bitNums)
{ return CalcOffsetTexSRTAry(bitNums) + sizeof(TexSRT) * bitNums.texSRT; }
u32 CalcOffsetGetAlphaCompare(const internal::MatMemNums& bitNums)
{ return CalcOffsetTexCoordGenAry(bitNums) + sizeof(TexCoordGen) * bitNums.texCoordGen; }
u32 CalcOffsetBlendMode(const internal::MatMemNums& bitNums)
{ return CalcOffsetGetAlphaCompare(bitNums) + sizeof(AlphaCompare) * bitNums.alpComp; }
}
BlendMode* Material::GetBlendModePtr()
{
    return internal::ConvertOffsToPtr<BlendMode>(this->m_pMem, CalcOffsetBlendMode(this->m_MemCap));
}
} }
