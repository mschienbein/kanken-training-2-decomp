/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/gfx/res/gfx_ResParticleAnimation.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include "../../ut/ut_ResUtil.h"
#include "../../ut/ut_ResDictionary.h"
#include "gfx_ResParticleAnimationOption.h"
#include "gfx_ResTypeInfo.h"

namespace nw  {
namespace gfx {
namespace res {

struct ResParticleAnimationData
{
    nw::ut::ResTypeInfo typeInfo;
    nw::ut::Offset toParticleAnimationOption;
    nw::ut::ResS32 m_AnimationLength;
    nw::ut::ResS32 m_AnimationDimension;
    nw::ut::ResS32 m_AnimationStride;
    nw::ut::ResS32 m_AnimationEnabledTableCount;
    nw::ut::Offset toAnimationEnabledTable;
    nw::ut::ResS32 m_AnimationDataTableCount;
    nw::ut::Offset toAnimationDataTable;
};

class ResParticleAnimation : public nw::ut::ResCommon<ResParticleAnimationData>
{
public:
    enum
{ TYPE_INFO = NW_GFX_RES_TYPE_INFO(ResParticleAnimation) };
    enum
{ SIGNATURE = NW_RES_SIGNATURE32('ANIM') };

    NW_RES_CTOR( ResParticleAnimation )


    ResParticleAnimationOption GetParticleAnimationOption() { return ResParticleAnimationOption(ref().toParticleAnimationOption.to_ptr()); }
    const ResParticleAnimationOption GetParticleAnimationOption() const { return ResParticleAnimationOption(ref().toParticleAnimationOption.to_ptr()); }

    s32 GetAnimationLength() const { return static_cast<s32>(ref().m_AnimationLength); }
    void SetAnimationLength(s32 value) { ref().m_AnimationLength = value; }

    s32 GetAnimationDimension() const { return static_cast<s32>(ref().m_AnimationDimension); }
    void SetAnimationDimension(s32 value) { ref().m_AnimationDimension = value; }

    s32 GetAnimationStride() const { return static_cast<s32>(ref().m_AnimationStride); }
    void SetAnimationStride(s32 value) { ref().m_AnimationStride = value; }

    s32 GetAnimationEnabledCount() const
        { return ref().m_AnimationEnabledTableCount; }
    bool GetAnimationEnabled(int idx) const
        {
            const bool* tbl_ptr = static_cast<const bool*>(ref().toAnimationEnabledTable.to_table_ptr());
            return tbl_ptr[idx];
        }
    void SetAnimationEnabled(int idx, bool value)
        {
            bool* tbl_ptr = GetAnimationEnabled();
            tbl_ptr[idx] = value;
        }
    bool* GetAnimationEnabled()
        { return static_cast<bool*>(ref().toAnimationEnabledTable.to_table_ptr()); }
    const bool* GetAnimationEnabled() const
        { return static_cast<const bool*>(ref().toAnimationEnabledTable.to_table_ptr()); }

    s32 GetAnimationDataCount() const
        { return ref().m_AnimationDataTableCount; }
    f32 GetAnimationData(int idx) const
        {
            const f32* tbl_ptr = static_cast<const f32*>(ref().toAnimationDataTable.to_table_ptr());
            return tbl_ptr[idx];
        }
    void SetAnimationData(int idx, f32 value)
        {
            f32* tbl_ptr = GetAnimationData();
            tbl_ptr[idx] = value;
        }
    f32* GetAnimationData()
        { return static_cast<f32*>(ref().toAnimationDataTable.to_table_ptr()); }
    const f32* GetAnimationData() const
        { return static_cast<const f32*>(ref().toAnimationDataTable.to_table_ptr()); }

    nw::ut::ResTypeInfo     GetTypeInfo() const { return ref().typeInfo; }
};

}
}
}

